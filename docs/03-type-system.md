## Mathium Type System – Design Spec (Target)

**Last Updated:** November 20, 2025

---

This document describes the *intended* design of Mathium’s type system.

The goals are:
- Everything is an object with a class.
- Classes are also objects.
- Every class ultimately derives from `MathObj`.
- Mathium supports both static and dynamic typing in a coherent way.

All terminology below is about the *Mathium language model*, not C++.

---

## 1. Core Runtime Model

### 1.1 Objects and Classes

- **Math object (`MathObj`)**: Root of the *class* hierarchy.
	- Every *class* in Mathium (including `MathObj` itself) is an instance of some class.
	- Equivalent to `object` in Python.

- **Class**: Describes the shape and behavior of Mathium objects.
	- Each class `C` is itself a *Math object* (i.e. an instance of another class).
	- Each class has:
		- A name.
		- Zero or more base classes (single inheritance initially, extensible to multiple/MRO later).
		- A table of methods and properties.
		- Rules for constructing instances and computing default values.
	- Instances of a class are called **runtime objects**.

- **Runtime object**: A concrete value at execution time.
	- Has an associated class `obj.__class__` (a `Class` instance).
	- Exposes properties/methods resolved via the class hierarchy.
	- Examples: an integer literal `5`, a real `3.14`, a function closure, a user‑defined struct instance.

#### 1.1.1 Special root classes

- `MathObj` (class):
	- Base class of *all* classes and objects.
	- Methods common to everything (e.g. `toString`, equality hooks) live here.

- `Class` (metaclass):
	- Class of all *class objects*.
	- Every user‑defined class and builtin class is an instance of `Class`.
	- `Class` itself is a subclass of `MathObj`.

- **Bootstrap invariants**:
	- `MathObj` is an instance of `Class`.
	- `Class` is an instance of itself (classic smalltalk/Python‑like metaclass loop), or of a dedicated metaclass if we later introduce one.
	- Every other class `C` is an instance of `Class` and (directly or indirectly) a subclass of `MathObj`.

At the C++ level, all runtime objects share a common base (`Object`).
`Class` is a concrete C++ subclass of `Object` that implements the Mathium class behavior.

---

## 2. Runtime vs Semantic Types

Mathium distinguishes **runtime classes** from **semantic types** used by the compiler.

### 2.1 Runtime Class (dynamic type)

- The **runtime class** of an object is what you get from `obj.__class__`.
- It fully describes how the object behaves at execution time: its methods, fields, and casting rules.
- In the C++ implementation this is represented by the `Class` hierarchy (`Integer`, `Real`, `Function`, etc.).

### 2.2 Semantic Type (static type)

- The **semantic type** is what the *semantic analyzer* uses during compilation.
- It is a pair:
	- `underlying_class` – a pointer/reference to a runtime `Class` object.
	- `qualifier` – modifiers like `const`, `ref`, `const ref`, possibly others later.
- Conceptually:

	- Semantic type  $T = (C, q)$,  where  $C$ is a runtime class and $q$ is a qualifier.

- Examples:
	- `Integer` → `(IntegerClass, NONE)`.
	- `const Integer` → `(IntegerClass, CONST)`.
	- `ref Real` → `(RealClass, REF)`.

The important rule: **there is exactly one runtime class object representing a given class**, and all semantic types that refer to that class reference the same `Class` instance.

---

## 3. Static and Dynamic Typing

Mathium aims to blend static and dynamic typing.

### 3.1 Static typing

- Variables, parameters, and function return values *may* have explicit types.
- The analyzer tracks a semantic type for:
	- Expressions.
	- Variables (declared or inferred).
	- Function parameters and return types.
- The compiler enforces that operations are type‑correct up to defined coercion rules.

### 3.2 Dynamic typing

- Values at runtime always carry a class.
- Even in untyped or loosely typed code, the VM can:
	- Inspect `obj.__class__`.
	- Dispatch on methods and operators dynamically.
- When the static type information is missing or imprecise, the analyzer permits more operations but may emit warnings or generate runtime checks.

### 3.3 Type coercion and overloading

- Mathium supports implicit type coercion between numeric types and, later, user types.
- Coercion has levels (for example):
	- `EXACT` – same class and qualifier.
	- `LOSSLESS` – safe widening (e.g. `Integer → Real`).
	- `LOSSY` – narrowing or precision loss.
	- `INCOMPATIBLE` – no valid coercion path.
- Function calls and operator applications are resolved by:
	- Inferring argument semantic types.
	- Ranking candidate overloads using class specificity + coercion levels.
	- Emitting warnings for lossy matches; errors for incompatible ones.

At runtime, casts are delegated to the `Class` hierarchy (e.g. `Class::cast`), which knows how to produce instances of the target class.

---

## 4. Builtin Types and Classes

Mathium ships with a small core of builtin classes:

- `None`
	- Singleton value representing “no value”.
	- Class `None` is a subclass of `MathObj`.

- `Integer`
	- Arbitrary‑precision integer using GMP.
	- Provides arithmetic operators, comparisons, and conversions.

- `Real`
	- Arbitrary‑precision real using MPFR.
	- Provides arithmetic operators and conversions.

- `Function`
	- Represents callable values (builtins and user‑defined).
	- Instances capture code pointers/bytecode plus closure environments.

- `Reference`
	- Represents indirections used for variable semantics (e.g. mutable bindings, ref parameters).

- `Class`
	- Metaclass for all classes, as described above.

All builtin classes are created and wired up during interpreter bootstrap.
The semantic layer also registers corresponding semantic types so the analyzer can reason about them statically.

---

## 5. User‑Defined Types and Classes

User code can define new classes which integrate with both the semantic and runtime systems.

### 5.1 Class declaration (language level)

Conceptually, a user‑defined class looks like:

```
class Point:
	let x: Real
	let y: Real

	fn length(none) -> Real:
		return sqrt(x ^ 2 + y ^ 2)
```

Design requirements:
- Every user class:
	- Is a subclass of `MathObj` (directly or indirectly).
	- Is an instance of the metaclass `Class`.
- Class bodies may declare:
	- Fields/properties.
	- Methods (normal and operator methods).

### 5.2 Semantic representation

During semantic analysis, a class declaration produces:
- A runtime `Class` object:
	- Name, base class, property and method tables.
- A semantic `Type` for the class:
	- `(class_object, qualifier)` with default qualifier `NONE`.

These are stored in:
- A **symbol table/scope** for names.
- The **type environment** used by the analyzer for later references.

### 5.3 Runtime behavior

- Creating an instance of a user class:
	- Allocates a new runtime `Object` whose `__class__` is the user `Class`.
	- Initializes fields to their default values (either explicit or from the class definition).
- Method dispatch:
	- Uses the class’s property/method registry and base classes to find the correct implementation.

---

## 6. Separation of Concerns

To keep the system maintainable, we enforce a clean separation:

### 6.1 Runtime layer

- Responsible for:
	- `Object` base type in C++ (storage and property access).
	- `Class` hierarchy and metaclass bootstrap.
	- Concrete builtin objects (`IntegerObj`, `RealObj`, etc.).
	- Casting and property lookup at runtime.
	- VM execution and memory management.

- Knows nothing about AST nodes or semantic analysis.

### 6.2 Semantic layer

- Responsible for:
	- Semantic `Type` abstraction (class + qualifier).
	- Type inference and checking.
	- Function and operator overload resolution.
	- Mapping AST nodes to semantic types.

- Refers to runtime classes only by stable handles (e.g. `ClassPtr`), not by concrete C++ subclasses directly.

### 6.3 Frontend layer

- Responsible for:
	- Tokens, AST, and parsing.
	- Class/type syntax (how user code declares types).

Frontend does not know about runtime object representations.

---

## 7. Bootstrap Sequence (High‑Level)

The interpreter starts up as follows:

1. **Create root classes**
	 - Allocate runtime class objects for `MathObj` and `Class`.
	 - Set up the metaclass loop:
		 - `MathObj.__class__ = Class`.
		 - `Class.__class__ = Class`.

2. **Create other builtin classes**
	 - `None`, `Integer`, `Real`, `Function`, `Reference`, etc.
	 - Set their base class to `MathObj` (or another builtin when appropriate).

3. **Initialize builtin methods and properties**
	 - Attach methods (including operator methods) and properties to each builtin class.

4. **Register semantic types**
	 - For each builtin class, create a corresponding semantic `Type` with default qualifier `NONE` and register it with the semantic type system.

5. **Expose builtins to user code**
	 - Insert builtin class objects and functions into the top‑level symbol table.

Once this sequence finishes, user code can define new classes and types using the same underlying mechanisms.

---

## 8. Future Extensions

The design intentionally leaves space for:
- Additional qualifiers (e.g. `mutable`, `owned`, `borrowed`).
- Parametric types / generics (e.g. `List[Integer]`).
- Interfaces or traits separate from concrete classes.
- Gradual typing features (e.g. an explicit `Dyn` type).

These should be modeled by extending the semantic `Type` abstraction, while keeping runtime `Class` focused on executable behavior.

---

This spec is the target behavior for the Mathium type system.
Implementation code should be refactored to match these concepts and separations.