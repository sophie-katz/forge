# Copyright (c) 2023-2024 Sophie Katz
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

echo -e "\033[0;96mexample.frg:3:5\033[0;90m: \033[1;31merror \033[0;90m[ES001]: \033[1;37mUnexpected ';', expected ')' or ','\033[0;0m"
echo -e "\033[0;90m3 |\033[0;0m print(\"hello, world\"\033[1;91m;\033[0;0m"
echo -e "\033[0;90m  |\033[0;0m                     \033[0;31m^\033[0;0m"
echo -e "\033[0;96mexample.frg:3:5\033[0;90m: \033[1;90msuggestion: \033[1;37mTry adding a ')' before the ';'\033[0;0m"
echo -e "\033[0;90m3 |\033[0;0m print(\"hello, world\"\033[1;92m)\033[0;0m;"
echo -e "\033[0;90m  |\033[0;0m                     \033[0;32m^\033[0;0m"
