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

# Generating valgrind suppression files

This guide is heavily based off of https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto.

## Identify a binary with memory leaks

Memory leaks are usually detected by the [full test wrapper](../../../forge-core/scripts/test_wrapper_full.bash) or by running the `forge` binary with `valgrind`.

Run a given command line with a Forge binary like so:

```shell
valgrind --tool=memcheck --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --suppressions=forge.supp --leak-check=full --show-leak-kinds=all <command>
```

If there is a memory leak reported, then there is an issue.

## Should the issue be suppressed?

You need to use your judgement about whether or not a given memory leak should be suppressed or fixed. "Still unreachable" memory leaks are [benign](https://stackoverflow.com/questions/67040349/valgrind-gives-error-memory-still-reachable) and if caused by a library can often be suppressed. "Indirectly lost" or "definitely lost" leaks are more serious and should be fixed.

Suppression is generally for false positives.

## How to suppress?

Run the same `valgrind` command as above, but with extra arguments to output a log file. Here's the full command:

```shell
valgrind --tool=memcheck --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --suppressions=forge.supp --leak-check=full --show-leak-kinds=all --gen-suppressions=all --error-limit=no --log-file=valgrind.log <command>
```

The log file will look something like this:

```
...
==29355== 16,384 bytes in 1 blocks are still reachable in loss record 9 of 9
==29355==    at 0x48850C8: malloc (vg_replace_malloc.c:381)
==29355==    by 0x491D48F: g_malloc (in /usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6)
==29355==    by 0x4927757: ??? (in /usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6)
==29355==    by 0x48DDFCB: ??? (in /usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6)
==29355==    by 0x400431B: call_init (dl-init.c:90)
==29355==    by 0x400431B: call_init (dl-init.c:27)
==29355==    by 0x4004423: _dl_init (dl-init.c:137)
==29355==    by 0x401A9B7: (below main) (dl-start.S:46)
==29355==
{
   <insert_a_suppression_name_here>
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   fun:g_malloc
   obj:/usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6
   obj:/usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6
   fun:call_init
   fun:call_init
   fun:_dl_init
   fun:(below main)
}
...
```

Strip out all lines starting with `==`, or any lines that are not in `{ ... }` blocks. The remaining file will be a series of suppression blocks:

```
...
{
   <insert_a_suppression_name_here>
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:calloc
   fun:g_malloc0
   obj:/usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6
   fun:g_slice_alloc
   fun:g_hash_table_new_full
   obj:/usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6
   obj:/usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6
   fun:call_init
   fun:call_init
   fun:_dl_init
   fun:(below main)
}
{
   <insert_a_suppression_name_here>
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   fun:g_malloc
   obj:/usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6
   obj:/usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.7400.6
   fun:call_init
   fun:call_init
   fun:_dl_init
   fun:(below main)
}
...
```

Use `*`, `?`, and `...` wildcards to make the suppression more general. Also replace the names. For example, one of the above suppressions could be made more general like this:

```
{
   <dl_init_0>
   Memcheck:Leak
   match-leak-kinds: reachable
   ...
   fun:g_malloc0
   obj:/usr/*lib*/libglib-2.0.so.*
   fun:g_slice_alloc
   fun:g_hash_table_new_full
   obj:/usr/*lib*/libglib-2.0.so.*
   obj:/usr/*lib*/libglib-2.0.so.*
   fun:call_init
   fun:call_init
   fun:_dl_init
   fun:(below main)
}
```

Then add/merge the new suppressions into the existing [`forge.supp`](../../../forge-core/forge.supp) file.

Then re-run the `valgrind` command to confirm it worked:

```shell
valgrind --tool=memcheck --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --suppressions=forge.supp --leak-check=full --show-leak-kinds=all <command>
```
