#!/bin/env python2
# -*- coding: utf-8 -*-

tab = '    '

### Field list in {"FieldName":'BitsLength'}
class CreateProtocol:
    def __init__(self, field_list, struct_name):
        self.struct_name = struct_name
        self.field_list = field_list

    def get_byte_order_network_to_host(self, size, string):
        if size > 8:
            return "ByteOrder::networkToHost(%s)" % string
        else:
            return string

    def get_byte_order_host_to_network(self, size, string):
        if size > 8:
            return "ByteOrder::hostToNetwork(%s)" % string
        else:
            return string

    def get_str_size(self, size):
        return 'uint%d_t' % size

    def set_first_lower_case(self, string):
        c = string[0].lower()
        string = c + string[1:]
        return string

    def get_set_function(self, field_name, size):
        return 'void set%s(%s %s) { getHeader().%s = %s; }' % (
                                                               field_name,
                                                               self.get_str_size(size),
                                                               self.set_first_lower_case(field_name),
                                                               self.set_first_lower_case(field_name),
                                                               self.get_byte_order_host_to_network(size, self.set_first_lower_case(field_name))
                                                               )

    def get_sets(self):
        set_functions = ''

        for field in self.field_list:
            set_functions += self.get_set_function(field[0], field[1]) + '\n'

        return set_functions

    def get_get_function(self, field_name, size):
        return '%s get%s() const{ return %s; }' % (
                                                               self.get_str_size(size),
                                                               field_name,
                                                               self.get_byte_order_network_to_host(size, "getHeader()." + self.set_first_lower_case(field_name))
                                                               )

    def get_gets(self):
        get_functions = ''

        for field in self.field_list:
            get_functions += self.get_get_function(field[0], field[1]) + '\n'

        return get_functions

    def get_struct_entry(self, entry_name, size):
        return '\n' + tab + '%s %s;' % (self.get_str_size(size),
                                        self.set_first_lower_case(entry_name))

    def get_struct(self):
        struct_entries = ''

        for field in self.field_list:
            struct_entries += self.get_struct_entry(field[0], field[1])

        return 'struct %s{%s\n}__attribute__((packed));' % (self.struct_name, struct_entries)

    def get_enum_entry(self, field_name):
        return '\n' + tab + '%s,' % (field_name)

    def get_enum(self):
        enum_entries = ''

        for field in self.field_list:
            enum_entries += self.get_enum_entry(field[0])

        enum_entries = enum_entries[:-1] + '\n'

        return 'enum IDs{%s};' % enum_entries

    def get_get_value_entry(self, field_name):
        return ('\n' + tab + 'case %s:\n' + tab*2 + 'tempValue = packet.get%s();\n' + tab*2 + 'break;') % (field_name, field_name)

    def get_get_value(self):
        get_value = ''

        for field in self.field_list:
            get_value += self.get_get_value_entry(field[0])

        return tab + 'switch(fieldID){%s\n    }' % (get_value)

# Usage: ./file.py <file> <StructName>
# File format: FieldName field bits size
# Example:
# MyField 32
def main(args):
    if len(args) is not 3:
        print "Usage: %s <file> <StructName>" % args[0]
        return
    f = open(args[1])

    field_list = []
    for line in f:
        line = line.split()
        line[1] = int(line[1])
        field_list.append(line)

    cp = CreateProtocol(field_list, args[2])

    print cp.get_gets()
    print
    print cp.get_sets()
    print
    print cp.get_struct()
    print


if __name__ == '__main__':
    from sys import argv
    main(argv);
