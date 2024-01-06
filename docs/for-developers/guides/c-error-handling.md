<!--
Copyright (c) 2024 Sophie Katz

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

# C error handling guidelines

## Die with internal error

## Log message

Messages can be emitted using the `frg_log_*~ functions. `

## Recovery

Functions that get into recoverable bad states can return statuses that describe their state. This is where the `frg_recoverable_status_t` enumerator comes into play.
