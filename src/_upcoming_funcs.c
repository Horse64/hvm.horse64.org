/* Copyright (c) 2023, ellie/@ell1e & HVM Team (see AUTHORS.md).

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
 */

/** @func io_get_working_dir
 *  @h64func io.get_working_dir@core.horse64.org
 */

/** @func io_open
 *  @h64func io.open@core.horse64.org
 */

/** @func io_exists
 *  @h64func io.exists@core.horse64.org
 */

/** @func io_is_dir
 *  @h64func io.is_dir@core.horse64.org
 */

/** @func io_list_dir
 *  @h64func io.list_dir@core.horse64.org
 */

/** @func io_make_or_get_appcache
 *  @h64func io.make_or_get_appcache@core.horse64.org
 */

/** @func io_remove_file
 *  @h64func io.remove_file@core.horse64.org
 */

/** @func io_remove_dir
 *  @h64func io.remove_dir@core.horse64.org
 */

/** @func io_rename
 *  @h64func io.rename@core.horse64.org
 */

/** @func io_make_tmpdir
 *  @h64func io.make_tmpdir@core.horse64.org
 */

/** @func json_dump
 *  @h64func json.dump@core.horse64.org
 */

/** @func json_parse
 *  @h64func json.parse@core.horse64.org
 */

/** @func base64_parse
 *  @h64func base64.parse@core.horse64.org
 */

/** @func base64_dump
 *  @h64func base64.dump@core.horse64.org
 */

/** @func net_lookup_name
 *  @h64func net.lookup_name@core.horse64.org"
 */

/** @h64type IOError@core.horse64.org
 ** @h64base BaseError@core.horse64.org
 */

/** @h64type NetworkIOError@core.horse64.org
 ** @h64base IOError@core.horse64.org
 */

/** @func path_normalize
 *  @h64func path.normalize@core.horse64.org
 */

/** @func path_join
 *  @h64func path.join@core.horse64.org
 */

/** @func path_basename
 *  @h64func path.basename@core.horse64.org
 */

/** @func path_dirname
 *  @h64func path.dirname@core.horse64.org
 */

/** @var process_args
 *  @h64var process.args@core.horse64.org
 */

/** @func process_run
 *  @h64func process.run@core.horse64.org
 */

/** @func random_gen_id
 *  @h64func random.gen_id@core.horse64.org
 */

/** @func system_exit
 *  @h64func system.exit@core.horse64.org
 */

/** @func system_osname
 *  @h64func system.osname@core.horse64.org
 */

/** @func system_program_compiled_with
 *  @h64func system.program_compiled_with@core.horse64.org
 */

/** @func system_program_licenses_as_list
 *  @h64func system.program_licenses_as_list@core.horse64.org
 */

/** @func sytem_program_version
 *  @h64func system.program_version@core.horse64.org
 */

/** @func system_self_exec_path
 *  @h64func system.self_exec_path@core.horse64.org
 */

/** @func terminal_get_line
 *  @h64func terminal.get_line@core.horse64.org
 */

/** @func terminal_open_input
 *  @h64func terminal.open_input@core.horse64.org
 */

/** @func text_pos_from_line_col
 *  @h64func text.pos_from_line_col@core.horse64.org
 */

/** @func text_code_to_bytes
 *  @h64func text.code_to_bytes@core.horse64.org
 */

/** @func text_from_code
 *  @h64func text.from_code@core.horse64.org
 */

/** @func text_glyph_codepoint_len
 *  @h64func text.glyph_codepoint_len@core.horse64.org
 */

/** @func text_full_glyphs_in_sub
 *  @h64func text.full_glyphs_in_sub@core.horse64.org
 */

/** @func text_code
 *  @h64func text.code@core.horse64.org
 */

/** @func textformat_outdent
 *  @h64func textformat.outdent@core.horse64.org
 */

/** @func threading_make_lock
 *  @h64func threading.make_lock@core.horse64.org
 */

/** @func time_sleep
 *  @h64func time.sleep@core.horse64.org
 */

/** @func time_ts
 *  @h64func time.ts@core.horse64.org
 */

/** @func uri_get_protocol
 *  @h64func uri.get_protocol@core.horse64.org
 */

/** @func uri_to_file_or_vfs_path
 *  @h64func uri.to_file_or_vfs_path@core.horse64.org
 */

/** @func uri_escape_path
 *  @h64func uri.escape_path@core.horse64.org
 */

/** @func uri_normalize
 *  @h64func uri.normalize@core.horse64.org
 */

/** @func uri_from_disk_path
 *  @h64func uri.from_disk_path@core.horse64.org
 */

/** @func uri_from_vfs_path
 *  @h64func uri.from_vfs_path@core.horse64.org
 */

/** @func wildcard_match
 *  @h64func wildcard.match@core.horse64.org
 */

