<!--
This program is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program. If not, see <https://www.gnu.org/licenses/>.

Copyright 2023-2024 Sophie Katz
-->

# File module structure

## External modules

```
their_module_name/
    their_module_name_if_dynamic.so.0.1.0
    their_module_name_if_static.a
    +public_submodule/
        +public_file.frgd
    index.frgd
```

## Internal module

```
my_module_name/
    private_submodule/
        private_file.frg
    +public_submodule/
        private_file.frg
        +public_file.frg
```

From `private_file.frg`:

```
import ..public_submodule.private_file.{my_function, my_struct};
```
