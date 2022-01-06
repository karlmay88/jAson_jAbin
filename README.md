# jAson_jAbin
jAson (**A** for Array) is a super-light weight json inspired **not
standard conformant** format for exchanging large scale arrays in binary and text format.

The approach is simple:

The file layout has only two structural components, `nodes` and
`arrays`. `nodes` have a user-defined name and may contain other
nodes, a free-text descriptor and/or exactly one `array`.
`array`s must contain 5 components:

* a type descriptor identified by 2 1-byte characters 'ty' which contains
  a 1-byte character providing information of the actual data type in
  the array which is either 'i' for integer or 'r' for real, or 'c'
  for character.
* a bit descriptor identified by 2 1-byte characters 'bi' which contains a
  64 bit integer of bits per array element
* a #dimension descriptor identified by 2 1-byte characters 'nd' which
  contains a 64 bit integer of number of dimensions of the array
* a dimension descriptor identified by 2 1-byte characters 'di' which
  contains a vector of 64 bit integers describing the size of each
  dimension
* a data descriptor identified by by 2 1-byte characters 'da' which
  contains the actual array.

Characters `{`, `}` and `:` must not occur in user defined node names.

In ascii a jAson file may look like:

```json
{
	de:{analysis results obtained on 01/01/2020}
	mydata:{
		de:{first run}
		myvec1:{
			de:{column positions}
			array:{
				ty:{i}
				bi:{64}
				nd:{2}
				di:{2
				2}
				da:{1
				2
				3
				4}
			}
		}
		myvec2:{
			array:{
				ty:{r}
				bi:{64}
				nd:{1}
				di:{2}
				da:{0.5
				0.2}
			}
		}
		myscalar:{
			array:{
				ty:{r}
				bi:{64}
				nd:{1}
				di:{1}
				da:{1.5}
			}
		}
	}
}
```
Descriptors `di` and `da` may contain more than one integer number. In that case for text files `newline` serves as a value separator.

Currently arrays are supposed to be in column-major order.

Interfaces are available for `C++` and `R`.

For both a tree stucture is built during, that is, the file is not read into a buffer first, saving memory and time.

## R interface

For writing requires a list as input and derives the content of all
array descriptors from the actual list object. Currently, when writing
to binary all integer descriptors and data are written in 64 bit.

## C++ interface

The current `C++` implementation requires sibling nodes to be unique with regard to the node name.

Templating allows for different storage modes for the integer array descriptors.

## ToDo

All interfaces currently lack support for character arrays. The
approach will be that for character arrays `di` will contain the
number of bytes in `da`. Actual words in `da` are null terminated and
can therefore be retrieved by the parser.

There is currently no facility to escape `{}:` in `de`.
