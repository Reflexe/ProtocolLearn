#!/bin/env python2
# -*- coding: utf-8 -*-

tab = '    '

'''
    binary_round(n) -> n

    returns the next binary power for n.
    eg. 1023 -> 1024.
'''
def binary_round(n):
    return (n << 1) & ~n

### Field list in {"FieldName":'BitsLength'}
class CreateProtocol:
    def __init__(self, field_list, struct_name):
        self.struct_name = struct_name
        self.field_list = field_list

        self.bit_field_size = 0
        self.bit_field_list = []
        self.bit_field_name = ''

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
        if size > 64:
            raise ValueError('size is greather than 64 which is not supported')
        if size == 1:
            return 'bool'
        if not self.is_byte_field_size(size):
            size = binary_round(size)
        if size == 4:
            size = 8;

        return 'uint%d_t' % size

    def set_first_lower_case(self, string):
        c = string[0].lower()
        string = c + string[1:]
        return string

    def is_bit_field_in_progress(self):
        return self.bit_field_size != 0

    def is_byte_field_size(self, size):
        return size in (8, 16, 32, 64)

    def is_bit_field_end(self):
        return self.is_byte_field_size(self.bit_field_size)

    def add_bit_field(self, field_name, size):
        if not self.bit_field_name:
            self.bit_field_name = self.set_first_lower_case(field_name)
        else:
            self.bit_field_name += field_name

        self.bit_field_list.append((field_name, self.bit_field_size, size))
        self.bit_field_size += size

    def get_bit_field_set(self, size):
        if size == 8:
            return 'BIT_FIELD_SET'
        else:
            return 'BIT_FIELD_SET_NET'

    def get_bit_field_get(self, size):
        if size == 8:
            return 'BIT_FIELD_GET'
        else:
            return 'BIT_FIELD_GET_NET'

    def get_bit_field_set_function(self, field_name, offset, size):
        string = 'void set%s(%s %s) ' % (field_name, self.get_str_size(size), self.set_first_lower_case(field_name))

        return string + '{ %s(getHeader().%s, %d, %d, %s); }' % (self.get_bit_field_set(self.bit_field_size), self.bit_field_name,
                                                                 offset, size, self.set_first_lower_case(field_name))

    def get_bit_field_sets(self):
        set_functions = ''

        for field in self.bit_field_list:
            set_functions += self.get_bit_field_set_function(field[0], field[1], field[2]) + '\n'

        return set_functions

    def clear_bit_field(self):
        self.bit_field_size = 0
        self.bit_field_list = []
        self.bit_field_name = ''

    def get_set_function(self, field_name, size):
        if not self.is_byte_field_size(size) or self.is_bit_field_in_progress():
            self.add_bit_field(field_name, size)

            if self.is_bit_field_end():
                string = self.get_bit_field_sets()[:-1] # delete the last '\n'
                self.clear_bit_field()

                return string
            else:
                return None
        else:
            string = 'void set%s(%s %s) ' % (field_name, self.get_str_size(size), self.set_first_lower_case(field_name))

            return string + '{ getHeader().%s = %s; }' % (self.set_first_lower_case(field_name),
                                                          self.get_byte_order_host_to_network(size, self.set_first_lower_case(field_name)))

    def get_sets(self):
        set_functions = ''

        for field in self.field_list:
            string = self.get_set_function(field[0], field[1])

            if string:
                set_functions += string + '\n'

        if self.is_bit_field_in_progress():
            raise ValueError('self.is_bit_field_in_progress() == True')

        return set_functions

    def get_bit_field_get_function(self, field_name, offset, size):
        string = '%s get%s() const' % (self.get_str_size(size), field_name)

        return string + '{ return %s(getHeader().%s, %d, %d); }' % (self.get_bit_field_get(self.bit_field_size), self.bit_field_name,
                                                                    offset, size)

    def get_bit_field_gets(self):
        get_functions = ''

        for field in self.bit_field_list:
            get_functions += self.get_bit_field_get_function(field[0], field[1], field[2]) + '\n'

        return get_functions


    def get_get_function(self, field_name, size):
        if not self.is_byte_field_size(size) or self.is_bit_field_in_progress():
            self.add_bit_field(field_name, size)

            if self.is_bit_field_end():
                string = self.get_bit_field_gets()[:-1] # delete the last '\n'
                self.clear_bit_field()

                return string
            else:
                return None
        else:
            return '%s get%s() const{ return %s; }' % (self.get_str_size(size), field_name,
                                                       self.get_byte_order_network_to_host(size, "getHeader()." + self.set_first_lower_case(field_name)))

    def get_gets(self):
        get_functions = ''

        for field in self.field_list:
            string = self.get_get_function(field[0], field[1])
            if string:
                get_functions += string + '\n'

        if self.is_bit_field_in_progress():
            raise ValueError('self.is_bit_field_in_progress() == True: %d')

        return get_functions

    def get_bit_field_struct_entry(self):
        return '\n' + tab + '%s %s;' % (self.get_str_size(self.bit_field_size),
                                        self.set_first_lower_case(self.bit_field_name))

    def get_struct_entry(self, entry_name, size):
        if not self.is_byte_field_size(size) or self.is_bit_field_in_progress():
            self.add_bit_field(entry_name, size)

            if self.is_bit_field_end():
                string = self.get_bit_field_struct_entry() # delete the last '\n'
                self.clear_bit_field()

                return string
            else:
                return None
        else:
            return '\n' + tab + '%s %s;' % (self.get_str_size(size),
                                            self.set_first_lower_case(entry_name))

    def get_struct(self):
        struct_entries = ''

        for field in self.field_list:
            string = self.get_struct_entry(field[0], field[1])

            struct_entries += string if string else ''

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
