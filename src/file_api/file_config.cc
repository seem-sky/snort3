//--------------------------------------------------------------------------
// Copyright (C) 2014-2016 Cisco and/or its affiliates. All rights reserved.
// Copyright (C) 2012-2013 Sourcefire, Inc.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------
/*
** Author(s):  Hui Cao <huica@cisco.com>
**
** NOTES
** 5.25.2012 - Initial Source Code. Hui Cao
*/

#include "file_config.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <iostream>
#include <sys/types.h>

#include "parser/parse_utils.h"
#include "main/snort_debug.h"
#include "main/snort_config.h"
#include "main/snort_types.h"

bool FileConfig::process_file_magic(FileMagicData &magic)
{
    bool negated = false;
    std::string str = '"' + magic.content_str + '"';

    if ( !parse_byte_code(str.c_str(), negated, magic.content) )
        return false;

    if (negated)
        return false;

    return true;
}

uint32_t FileConfig::find_file_type_id(const uint8_t* buf, int len,
    uint64_t file_offset, void** context)
{
    return fileIdentifier.find_file_type_id(buf, len, file_offset, context);
}

/*The main function for parsing rule option*/
void FileConfig::process_file_rule(FileMagicRule &rule)
{
    fileIdentifier.insert_file_rule(rule);
}

void FileConfig::process_file_policy_rule(FileRule &rule)
{
    filePolicy.insert_file_rule(rule);
}

FileMagicRule*  FileConfig::get_rule_from_id(uint32_t id)
{
    return fileIdentifier.get_rule_from_id(id);
}

std::string FileConfig::file_type_name( uint32_t id)
{
    if (SNORT_FILE_TYPE_UNKNOWN == id)
        return "Unknown file type, done";

    else if (SNORT_FILE_TYPE_CONTINUE == id)
        return "Undecided file type, continue...";

    FileMagicRule* info = get_rule_from_id(id);

    if (info != nullptr)
        return info->type;

    return "";
}

std::string file_type_name(uint32_t id)
{
    return snort_conf->file_config.file_type_name(id);
}
