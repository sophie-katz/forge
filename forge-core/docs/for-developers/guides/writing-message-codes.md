<!--
Copyright (c) 2023-2024 Sophie Katz

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

# Writing message codes

Message codes go in the `forge/messages/codes.h` header file. They are used to provide consistency in how messages are displayed to users and to have an index of unique codes to identify them.

These guidelines should be followed when writing message codes:

- See [Message codes](https://sophie-katz.github.io/forge/reference/message-codes/) for instructions on how to choose codes for messages.

- **Capitalize:** Messages should start with a capital letter and not end with punctuation. This is to help with readability.

- **Add context:** Messages that are context dependent should make it very clear what the context is. Stating the source range is a great way to do this, or by providing a syntax tree backtrace.

- **Include values:** Messages that are caused by specific values should include those values in the message. For example, if an error is caused because a number goes over a certain limit both the number and the limit should be present in the message text.

- **Fail quickly:** Errors should be thrown immediately after the error is detected. This is to help with debugging.

- **Avoid double negatives:** For example, instead of saying "do not use invalid characters" say "only use valid characters".

- **Be solution oriented:** Instructs the users on how to fix the problem wherever possible.
