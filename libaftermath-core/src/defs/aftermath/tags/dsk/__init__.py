# Author: Andi Drebes <andi@drebesium.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as published
# by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
# USA.

from aftermath.tags import Tag, FunctionTag, GenerateFunctionTag, TemplatedGenerateFunctionTag, Compound
from aftermath.util import enforce_type
import aftermath

class ReadFunction(FunctionTag):
    """Function reading an instance of the associated type from disk"""

    def __init__(self, function_name = None):
        super(ReadFunction, self).__init__(function_name = function_name,
                                           default_suffix = "_read")

class GenerateReadFunction(TemplatedGenerateFunctionTag, ReadFunction):
    """Generate a ReadFunction"""

    def __init__(self, function_name = None):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.ReadFunction)
        ReadFunction.__init__(self, function_name = function_name)

class GenerateArrayReadFunction(TemplatedGenerateFunctionTag, ReadFunction):
    """Generate a ReadFunction thad reads a array of elements"""

    def __init__(self,
                 num_elements_field_name,
                 array_field_name,
                 verbatim_field_names,
                 function_name = None):
        """`Num_elements_field` is the name of the field that contains the number of
        elements to be read. `Array_field_name` is the name of the field that
        will point to the array's elements after reading the elements from
        disk. `verbatim_field_names` is a list of names of fields that are to be
        read verbatim from disk. Their order specifies the order in which they
        are read from disk. This list must contain num_elements_field_name."""

        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.ArrayReadFunction)
        ReadFunction.__init__(self, function_name = function_name)

        enforce_type(num_elements_field_name, str)
        enforce_type(array_field_name, str)

        if not num_elements_field_name in verbatim_field_names:
            raise Exception("The field '" + num_elements_field_name + " with " +
                            "the number of elements must appear in list of " +
                            "fields to be read verbatim")

        self.__num_elements_field_name = num_elements_field_name
        self.__array_field_name = array_field_name
        self.__verbatim_field_names = verbatim_field_names

    def getNumElementsField(self):
        """Returns the field that contains the number of elements of the array"""

        return self.getType().getFields().getFieldByName(self.__num_elements_field_name)

    def getArrayField(self):
        """Returns the field that contains the array elements"""

        return self.getType().getFields().getFieldByName(self.__array_field_name)

    def getVerbatimFields(self):
        """Returns a list of fields that should be copied verbatim (i.e., that
        are not involved) in the array functionality"""

        return map(lambda fname: self.getType().getFields().getFieldByName(fname),
                   self.__verbatim_field_names)

class DumpStdoutFunction(FunctionTag):
    """Function dumping the contents of an instance of the associated on-disk
    type to stdout"""

    def __init__(self, function_name = None):
        super(DumpStdoutFunction, self).__init__(function_name = function_name,
                                                 default_suffix = "_dump_stdout")

class GenerateDumpStdoutFunction(TemplatedGenerateFunctionTag,
                                 DumpStdoutFunction):
    """Generate a DumpStdoutFunction"""

    def __init__(self, function_name = None):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.DumpStdoutFunction)
        DumpStdoutFunction.__init__(self, function_name = function_name)

class WriteFunction(FunctionTag):
    """Function writing an instance of the associated type to disk."""

    def __init__(self, function_name = None, has_type_param = False):
        """If `has_type_param` is true, an extra parameter with the numerical
        value for the on-disk frame is required to invoke this function."""

        super(WriteFunction, self).__init__(function_name = function_name,
                                            default_suffix = "_write")
        self.__has_type_param = has_type_param

    def hasTypeParam(self):
        return self.__has_type_param

    def setTypeParam(self, b):
        enforce_type(b, bool)

        self.__has_type_param = b

class GenerateWriteFunction(TemplatedGenerateFunctionTag, WriteFunction):
    """Generate a WriteFunction"""

    def __init__(self, function_name = None, has_type_param = False):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.WriteFunction)
        WriteFunction.__init__(self,
                               function_name = function_name,
                               has_type_param = has_type_param)

class LoadFunction(FunctionTag):
    """Function that loads one instance of the associated type from disk, then
    invoking the assertion and postprocessing function (if defined for the
    type)"""

    def __init__(self, function_name = None):
        super(LoadFunction, self).__init__(function_name = function_name,
                                           default_suffix = "_load")

class GenerateLoadFunction(TemplatedGenerateFunctionTag, LoadFunction):
    """Generate a LoadFunction"""

    def __init__(self, function_name = None):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.LoadFunction)
        LoadFunction.__init__(self, function_name = function_name)

class Frame(Tag):
    """Indicates that an on-disk structure is a frame (a data structure preceded by
    a numerical identifier for it's type)."""

    pass

class WriteToBufferFunction(FunctionTag):
    """Function that converts the in-memory representation of the on-disk type into
    the final on-disk representation ready to be written to disk and writes the
    result into the am_write_buffer passed as an argument.
    """

    def __init__(self,
                 function_name = None,
                 has_type_param = False,
                 constant_type_id = None):
        """If `has_type_param` is true, an extra parameter with the numerical
        value for the on-disk frame is required to invoke this function. If
        `constant_type_id` is given, the numerical value for the on-disk frame
        is set automatically."""

        super(WriteToBufferFunction, self).__init__(function_name = function_name,
                                                    default_suffix = "_write_to_buffer")
        enforce_type(has_type_param, bool)
        enforce_type(constant_type_id, [int, type(None)])

        self.__has_type_param = has_type_param
        self.__constant_type_id = constant_type_id
        self.checkTypeCompat()

    def checkTypeCompat(self):
        if self.hasConstantTypeID() and self.hasTypeParam():
            raise Exception("Cannot have a type parameter and a constant type "
                            "ID at the same time.")

    def hasTypeParam(self):
        return self.__has_type_param

    def setTypeParam(self, b):
        enforce_type(b, bool)

        self.__has_type_param = b
        self.checkTypeCompat()

    def hasConstantTypeID(self):
        return not (self.__constant_type_id is None)

    def setConstantTypeID(self, i):
        enforce_type(i, int)

        self.__constant_type_id = i
        self.checkTypeCompat()

    def getConstantTypeID(self):
        return self.__constant_type_id

class GenerateWriteToBufferFunction(TemplatedGenerateFunctionTag,
                                    WriteToBufferFunction):
    """Generate a WriteToBufferFunction"""

    def __init__(self, function_name = None, has_type_param = False):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.WriteToBufferFunction)
        WriteToBufferFunction.__init__(self,
                                       function_name = function_name,
                                       has_type_param = has_type_param)

    def instantiateTemplate(self):
        # If this is a compound type, the types of all fields must also define a
        # WriteToBufferFunction, since this function is invoked for each field.

        if self.getType().isCompound():
            for field in self.getType().getFields():
                if not field.getType().getTagInheriting(WriteToBufferFunction):
                    raise Exception("Cannot generate WriteToBufferFunction for "
                                    "compound type "
                                    "'" + self.getType().getName() + "': Field "
                                    "'" + field.getName() + "' has type "
                                    "'" + field.getType().getName() + "', which "
                                    "does not have a WriteToBufferFunction.")

        return TemplatedGenerateFunctionTag.instantiateTemplate(self)

class WriteToBufferWithDefaultIDFunction(FunctionTag):
    """Function that converts the in-memory representation of the on-disk type
    into the final on-disk representation ready to be written to disk and writes
    the result into the am_write_buffer passed as an argument using the default
    on-disk type ID for the type.
    """

    def __init__(self, function_name = None):
        super(WriteToBufferWithDefaultIDFunction, self).__init__(
            function_name = function_name,
            default_suffix = "_write_to_buffer_defid")

class GenerateWriteToBufferWithDefaultIDFunction(TemplatedGenerateFunctionTag,
                                                 WriteToBufferWithDefaultIDFunction):
    """Generate a WriteToBufferWithDefaultIDFunction"""

    def __init__(self):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.WriteToBufferWithDefaultIDFunction)

        WriteToBufferWithDefaultIDFunction.__init__(self)

class WriteDefaultIDToBufferFunction(FunctionTag):
    """Function that writes a frame type id structure with the default ID for the
    type to a buffer.
    """

    def __init__(self, function_name = None):
        super(WriteDefaultIDToBufferFunction, self).__init__(
            function_name = function_name,
            default_suffix = "_write_default_id_to_buffer")

class GenerateWriteDefaultIDToBufferFunction(TemplatedGenerateFunctionTag,
                                                 WriteDefaultIDToBufferFunction):
    """Generate a WriteDefaultIDToBufferFunction"""

    def __init__(self):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.WriteDefaultIDToBufferFunction)

        WriteDefaultIDToBufferFunction.__init__(self)

class WriteDefaultIDFunction(FunctionTag):
    """Function that writes a frame type id structure with the default ID for the
    type to an output file using an I/O context.
    """

    def __init__(self, function_name = None):
        super(WriteDefaultIDFunction, self).__init__(
            function_name = function_name,
            default_suffix = "_write_default_id")

class GenerateWriteDefaultIDFunction(TemplatedGenerateFunctionTag,
                                     WriteDefaultIDFunction):
    """Generate a WriteDefaultIDFunction"""

    def __init__(self):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.WriteDefaultIDFunction)

        WriteDefaultIDFunction.__init__(self)

class WriteWithDefaultIDFunction(FunctionTag):
    """Function that converts the in-memory representation of the on-disk type
    into the final on-disk representation and writes the result to disk using an
    output context with the default on-disk type ID for the type.
    """

    def __init__(self, function_name = None):
        super(WriteWithDefaultIDFunction, self).__init__(
            function_name = function_name,
            default_suffix = "_write_defid")

class GenerateWriteWithDefaultIDFunction(TemplatedGenerateFunctionTag,
                                         WriteWithDefaultIDFunction):
    """Generate a WriteWithDefaultIDFunction"""

    def __init__(self):
        TemplatedGenerateFunctionTag.__init__(
            self,
            template_type = aftermath.templates.dsk.WriteWithDefaultIDFunction)

        WriteWithDefaultIDFunction.__init__(self)
