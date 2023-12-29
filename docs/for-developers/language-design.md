<!--
Copyright (c) 2023 Sophie Katz

This file is part of Forge.

Forge is free software: you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation, either version
3 of the License, or (at your option) any later version.

Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Forge. If
not, see <https://www.gnu.org/licenses/>.
-->

# Goals

- Native
- Only one way to do things
- Don't have to worry about things unless you want to fine-tune
- AI integration
- Good integration with other programming languages

# Typing

- There is a default dynamic type with more efficient and stronger types

# Memory

- Memory is managed by scope, as shown by this pseudocode:

  ```
  // legal:
  {
    declare variable x

    // do something with reference to x
  }

  // illegal:
  {
    {
        declare variable x
    }

    // do something with reference to x
  }
  ```

  To implement this, the following rules must be followed:

  - Functions cannot return references to objects created in their scope
  - Objects can only contain references to other objects in the same pool

- Pools

# Objects

- Interfaces? Abstract classes? Structs?
- Algebraic data types
    - Can do algebra with data types
    - You can pass in data types with duck-typed constrants
    - Data types have interfaces
    - Any interface can be fit to any data

# Example

```
// Declare a data type
ty Person {
    name: string;
    age: integer;
}

iface Introduce: Person {
    mut fn introduce(const self) -> str {
        return "Hello, my name is {self.name} and I am {self.age} years old.";
    }
}

ty School = ::HáskóliÍslands
    | ::HáskólinnÍReykjavík;

iface GetSchoolName {
    prop school: const School;

    fn getSchoolName(const self) -> str {
        return match self.school {
            School::HáskóliÍslands => "Háskóli Íslands",
            School::HáskólinnÍReykjavík => "Háskólinn í Reykjavík",
        };
    }
}

ty Student = Person + GetSchoolName {
    school: School;

    fn @init(const self) {
        print("Student {self.name} created.");
    }
}

iface IntroduceStudent: Student {
    override fn introduce(const self) -> str {
        mut personIntroduction = super.introduce();

        return "{personIntroduction} I go to {self.getSchoolName()}.";
    }
}

fn main() {
    const student = Student {
        name: "Bob",
        age: 20,
        school: School::HáskóliÍslands
    };

    // Introduce as a student
    print(student.introduce());

    // Introduce as a person
    print(student.Person::introduce());
}
```

# Grammar

```
# This is a comment

# thisIsAFunctionOrVariableName

# ThisIsATypeName

# This is a type alias
ty A = B;

# This is a type union
ty A = [
    B: C,
    D: D,
];

# This is a struct
ty A = {
    b: B,
    c: C,

    fn @init() {}
}

# This is a type sum
ty A = {
    ...B,
    ...C,
};

# This is an optional type
ty A = B?;

# This is a non-optional type
ty A = B!;

# This is a type override
ty = AllOptional {
    a: A?,
    b: B?,
}

ty OneNonOptional = {
    ...AllOptional,
    a!,
}

# This is an enum
ty A = [
    B,
    C,
]

# This is an interface
iface A {}

# This is an interface for a type
iface A : B {
    # This is a required property
    prop c: C;

    # This is a static function
    fn f() -> D {}

    # This is a constant member function
    fn f(const self) -> D {}

    # This is a non-constant member function
    fn f(mut self) -> D {}

    # This is a static function that can be overridden
    mut f() -> D {}

    # This is a member function that can be overridden
    mut f(const self) -> D {}

    # This is an override
    override g() -> D {}
}

# This is an abstract interface
abstract iface A {
    fn f() -> D;

    fn f(const self) -> D;
}

# This is a function
fn f() {}

# This is a function with a return type
fn f() -> str {}

# This is a function with positional arguments
fn f(a: A, b: B) {}

# This is a function with keyword arguments
fn f(kw a: A, kw b: B) {}

# This is a function with optional arguments
fn f(a: A = 1) {}

# This is a function with variadic arguments
fn f(a: A...) {}

# This is a function with variadic keyword
fn f(a: A..., kw b: B...) {}
```

# Simplified AST

Simplified AST is a subset of the full grammar that lacks any syntactic sugar. Every aspect of the simplified AST can be passed directly to the interpreter or to codegen.

It contains:
- Signed and unsigned integer types
- Floating point types
- Structs
- Simple unions
- Functions without variadic or keyword arguments
- Return, if, while control flow constructs
- Unsigned integer literals
- Float literals
- Struct member access
- All operators (except compound assignment operators)
