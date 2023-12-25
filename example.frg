# Copyright (c) 2023 Sophie Katz
#
# This file is part of Forge.
#
# Forge is free software: you can redistribute it and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# Forge is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with Forge. If
# not, see <https://www.gnu.org/licenses/>.

ty Person {
    name: str,
    age: int,
}

iface Introduce: Person {
    mut fn introduce(const self) -> str {
        return "Hello, my name is {self.name} and I am {self.age} years old.";
    }
}

ty School = HáskóliÍslands
    | HáskólinnÍReykjavík;

iface GetSchoolName {
    prop school: const School;

    fn getSchoolName(const self) -> str {
        return match self.school {
            School::HáskóliÍslands -> "Háskóli Íslands",
            School::HáskólinnÍReykjavík -> "Háskólinn í Reykjavík",
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

    # Introduce as a student
    print(student.introduce());

    # Introduce as a person
    print(student.Person::introduce());
}
