/* Copyright (c) 2023-2024, ellie/@ell1e & HVM Team (see AUTHORS.md).

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Alternatively, at your option, this file is offered under the Apache 2
license, see accompanied LICENSE.md.
*/

#include "compileconfig.h"

/* NOTE: until we got around to actually implementing them where they're
supposed to be, list the built-in functions here so horsec can pick them
up for use. */

/** @func bignum_compare_nums
 *  @h64func bignum.compare_nums@core.horse64.org
 *  @is_later no
 */

/** @func io_make_dir
 *  @h64func io.make_dir
 *  @is_later yes
 */

/** @func io_working_dir
 *  @h64func io.working_dir@core.horse64.org
 *  @is_later yes
 */

/** @func io_open
 *  @h64func io.open@core.horse64.org
 *  @is_later yes
 */

/** @func io_exists
 *  @h64func io.exists@core.horse64.org
 *  @is_later yes
 */

/** @func io_is_dir
 *  @h64func io.is_dir@core.horse64.org
 *  @is_later yes
 */

/** @func io_list_dir
 *  @h64func io.list_dir@core.horse64.org
 *  @is_later yes
 */

/** @func io_tree_list_walker
 *  @h64func io.tree_list_walker@core.horse64.org
 *  @is_later yes
 */

/** @func io_make_or_get_appcache
 *  @h64func io.make_or_get_appcache@core.horse64.org
 *  @is_later yes
 */

/** @func io_remove_file
 *  @h64func io.remove_file@core.horse64.org
 *  @is_later yes
 */

/** @func io_remove_dir
 *  @h64func io.remove_dir@core.horse64.org
 *  @is_later yes
 */

/** @func io_rename
 *  @h64func io.rename@core.horse64.org
 *  @is_later yes
 */

/** @func io_make_tmpdir
 *  @h64func io.make_tmpdir@core.horse64.org
 *  @is_later yes
 */

/** @func json_dump
 *  @h64func json.dump@core.horse64.org
 *  @is_later no
 */

/** @func json_parse
 *  @h64func json.parse@core.horse64.org
 *  @is_later no
 */

/** @func base64_parse
 *  @h64func base64.parse@core.horse64.org
 *  @is_later no
 */

/** @func base64_dump
 *  @h64func base64.dump@core.horse64.org
 *  @is_later no
 */

/** @func net_fetch_open
 *  @h64func net.fetch.open@core.horse64.org
 *  @is_later yes
 */

/** @func net_lookup_name
 *  @h64func net.lookup_name@core.horse64.org
 *  @is_later yes
 */

/** @h64type io.IOError@core.horse64.org
 ** @h64base BaseError@core.horse64.org
 */

/** @h64type io.PathNotFoundError@core.horse64.org
 ** @h64base io.IOError@core.horse64.org
 */

/** @h64type net.NetworkIOError@core.horse64.org
 ** @h64base io.IOError@core.horse64.org
 */

/** @func path_sep
 *  @h64var path.sep@core.horse64.org
 */

/** @func path_normalize
 *  @h64func path.normalize@core.horse64.org
 *  @is_later no
 */

/** @func path_make_abs
 *  @h64func path.make_abs@core.horse64.org
 *  @is_later no
 */

/** @func path_join
 *  @h64func path.join@core.horse64.org
 *  @is_later no
 */

/** @func path_basename
 *  @h64func path.basename@core.horse64.org
 *  @is_later no
 */

/** @func path_dirname
 *  @h64func path.dirname@core.horse64.org
 *  @is_later no
 */

/** @var process_args
 *  @h64var process.args@core.horse64.org
 */

/** @func process_run
 *  @h64func process.run@core.horse64.org
 *  @is_later yes
 */

/** @func random_gen_id
 *  @h64func random.gen_id@core.horse64.org
 *  @is_later no
 */

/** @func system_exit
 *  @h64func system.exit@core.horse64.org
 *  @is_later no
 */

/** @var system_osname
 *  @h64var system.osname@core.horse64.org
 */

/** @var system_program_compiler_name
 *  @h64var system.program_compiler_name@core.horse64.org
 */

/** @func system_program_licenses_as_list
 *  @h64func system.program_licenses_as_list@core.horse64.org
 *  @is_later yes
 */

/** @var sytem_program_version
 *  @h64var system.program_version@core.horse64.org
 */

/** @func system_self_exec_path
 *  This function determines the filesystem path where thi
 *  program's own executable is currently located.
 *
 *  **Note:** On systems like Linux where the executable
 *  can be moved around at any point, it is **dangerous**
 *  to open this path and expect it to work. The reason is that
 *  between calling this function to obtain path, and then
 *  actually opening the target with @{io.open} or similar,
 *  the executable might have been renamed again. Therefore,
 *  don't use this in security sensitive contexts.
 *  (A common unproblematc use case is to simply be able to
 *  tell the user what the executable file name is.)
 *
 *  @h64func system.self_exec_path@core.horse64.org
 *  @is_later yes
 */

/** @func terminal_get_line
 *  @h64func terminal.get_line@core.horse64.org
 *  @is_later yes
 */

/** @func terminal_open_input
 *  @h64func terminal.open_input@core.horse64.org
 *  @is_later yes
 */

/** @func text_pos_from_line_col
 *  @h64func text.pos_from_line_col@core.horse64.org
 *  @is_later no
 */

/** @func text_code_to_bytes
 *  @h64func text.code_to_bytes@core.horse64.org
 *  @is_later no
 */

/** @func text_from_code
 *  @h64func text.from_code@core.horse64.org
 *  @is_later no
 */

/** @func text_glyph_codepoint_len
 *  @h64func text.glyph_codepoint_len@core.horse64.org
 *  @is_later no
 */

/** @func text_full_glyphs_in_sub
 *  @h64func text.full_glyphs_in_sub@core.horse64.org
 *  @is_later no
 */

/** @func text_code
 *  @h64func text.code@core.horse64.org
 *  @is_later no
 */

/** @func math_max
 *  @h64func math.max@core.horse64.org
 *  @is_later no
 */

/** @func math_min
 *  @h64func math.min@core.horse64.org
 *  @is_later no
 */

/** @func math_parse_hex
 *  @h64func math.parse_hex@core.horse64.org
 *  @is_later no
 */

/** @func math_floor
 *  @h64func math.floor@core.horse64.org
 *  @is_later no
 */

/** @func math_round
 *  @h64func math.round@core.horse64.org
 *  @is_later no
 */

/** @func math_abs
 *  @h64func math.abs@core.horse64.org
 *  @is_later no
 */

/** @func textformat_outdent
 *  @h64func textformat.outdent@core.horse64.org
 *  @is_later no
 */

/** @func threading_make_lock
 *  @h64func threading.make_lock@core.horse64.org
 *  @is_later no
 */

/** @func time_sleep
 *  @h64func time.sleep@core.horse64.org
 *  @is_later yes
 */

/** @func time_ticks
 *  @h64func time.ticks@core.horse64.org
 *  @is_later no
 */

/** @func uri_get_protocol
 *  @h64func uri.get_protocol@core.horse64.org
 *  @is_later no
 */

/** @func uri_to_file_or_vfs_path
 *  @h64func uri.to_file_or_vfs_path@core.horse64.org
 *  @is_later no
 */

/** @func uri_escape_path
 *  @h64func uri.escape_path@core.horse64.org
 *  @is_later no
 */

/** @func uri_normalize
 *  @h64func uri.normalize@core.horse64.org
 *  @is_later no
 */

/** @func uri_basename
 *  @h64func uri.basename@core.horse64.org
 *  @is_later no
 */

/** @func uri_dirname
 *  @h64func uri.dirname@core.horse64.org
 *  @is_later no
 */

/** @func uri_add_part
 *  @h64func uri.add_part@core.horse64.org
 *  @is_later no
 */

/** @func uri_from_disk_path
 *  @h64func uri.from_disk_path@core.horse64.org
 *  @is_later no
 */

/** @func uri_from_vfs_path
 *  @h64func uri.from_vfs_path@core.horse64.org
 *  @is_later no
 */

/** @func uri_traverse_up
 *  @h64func uri.traverse_up@core.horse64.org
 *  @is_later no
 */

/** @func uri_encode_path
 *  @h64func uri.encode_path@core.horse64.org
 *  @is_later no
 */

/** @func uri_unencode_path
 *  @h64func uri.unencode_path@core.horse64.org
 *  @is_later no
 */

/** @func wildcard_match
 *  @h64func wildcard.match@core.horse64.org
 *  @is_later no
 */

/** @func debugger_get_addr
 *  @h64func debugger.get_addr@core.horse64.org
 *  @is_later no
 */

/** @var debugger_is_debug_build
 *  @h64var debugger.is_debug_build@core.horse64.org
 *  @is_later no
 */

/** @func textfmt_template_from_str
 *  @h64func textfmt.template_from_str@core.horse64.org
 *  @is_later no
 */

/** @func net_serve_http
 *  @h64func net.serve.http@horse64.org
 *  @is_later no
 */

