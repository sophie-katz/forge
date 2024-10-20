#!/bin/bash

set -e

# echo "== FULLY STYLED =="
# echo

printf "\033[0;36mtest.frg:20:6:\033[0;0m \033[0;31merror ES001\033[0;0m \033[0;90m-\033[0;0m \033[1mIf statements must have conditions of type 'bool', not 'i32'\033[0;0m\n"
echo
printf "  \033[0;90m 20▕\033[0;0m if \033[1;31m5\033[0;0m {\n"
printf "    \033[0;90m ▕    \033[0;31m▔\033[0;0m\n"
echo
echo

printf "\033[0;36mtest.frg:132:6:\033[0;0m \033[0;33mwarning WS023 \033[0;90m-\033[0;0m \033[1mConditions in if statements do not need to be between '(' ')'\033[0;0m\n"
echo
printf "  \033[0;90m132▕\033[0;0m if \033[1;33m(x < 5)\033[0;0m {\n"
printf "     \033[0;90m▕    \033[0;33m▔▔▔▔▔▔▔\033[0;0m\n"
echo
printf "  \033[0;32msuggested fix \033[0;90m-\033[0;0m \033[1mRemove '(' and ')'\033[0;0m\n"
echo
printf "  \033[0;90m132▕\033[0;0m if \033[1;32mx < 5\033[0;0m {\n"
printf "     \033[0;90m▕    \033[0;32m▔▔▔▔▔\033[0;0m\n"
echo

# echo "== WITHOUT UNICODE =="
# echo

# printf "\033[0;36mtest.frg\033[0;0m\033[0;34m:20:6:\033[0;0m \033[0;31merror ES001\033[0;0m \033[0;90m-\033[0;0m \033[1mIf statements must have conditions of type 'bool', not 'i32'\033[0;0m\n"
# echo
# printf "  \033[0;90m 20|\033[0;0m if \033[1;31m5\033[0;0m {\n"
# printf "    \033[0;90m |    \033[0;31m^\033[0;0m\n"
# echo
# echo

# printf "\033[0;36mtest.frg\033[0;0m\033[0;34m:132:6:\033[0;0m \033[0;33mwarning WS023 \033[0;90m-\033[0;0m \033[1mConditions in if statements do not need to be between '(' ')'\033[0;0m\n"
# echo
# printf "  \033[0;90m132|\033[0;0m if \033[1;33m(x < 5)\033[0;0m {\n"
# printf "     \033[0;90m|    \033[0;33m^^^^^^^\033[0;0m\n"
# echo
# printf "  \033[0;32msuggestion \033[0;90m-\033[0;0m \033[1mRemove '(' and ')'\033[0;0m\n"
# echo
# printf "  \033[0;90m132|\033[0;0m if \033[1;32mx < 5\033[0;0m {\n"
# printf "     \033[0;90m|    \033[0;32m^^^^^\033[0;0m\n"
# echo

# echo "== WITHOUT COLOR =="
# echo

# printf "test.frg:20:6: error ES001 - If statements must have conditions of type 'bool', not 'i32'\n"
# echo
# printf "   20▕ if 5 {\n"
# printf "     ▕    ▔\n"
# echo
# echo

# printf "test.frg:132:6: warning WS023 - Conditions in if statements do not need to be between '(' ')'\n"
# echo
# printf "  132▕ if (x < 5) {\n"
# printf "     ▕    ▔▔▔▔▔▔▔\n"
# echo
# printf "  suggestion - Remove '(' and ')'\n"
# echo
# printf "  132▕ if x < 5 {\n"
# printf "     ▕    ▔▔▔▔▔\n"
# echo

# echo "== WITHOUT EITHER =="
# echo

# printf "test.frg:20:6: error ES001 - If statements must have conditions of type 'bool', not 'i32'\n"
# echo
# printf "   20| if 5 {\n"
# printf "     |    ^\n"
# echo
# echo

# printf "test.frg:132:6: warning WS023 - Conditions in if statements do not need to be between '(' ')'\n"
# echo
# printf "  132| if (x < 5) {\n"
# printf "     |    ^^^^^^^\n"
# echo
# printf "  suggestion - Remove '(' and ')'\n"
# echo
# printf "  132| if x < 5 {\n"
# printf "     |    ^^^^^\n"
# echo
