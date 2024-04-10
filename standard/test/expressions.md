# Expressions Tests

### Scope

The tests in this file verify that the interpreter analyzes expressions correctly (and provides the correct error messages when needed). This includes testing that each operator works as expected individually (unit tests) as well as testing that operators can be combined with the correct precedence and associativity (integration tests).

### Unit Tests

* `and`

| Input | Correct Output |
| ----- | -------------- |
| `True and True` | `True` |
| `True and False` | `False` |
| `False and True` | `False` |
| `False and False` | `False` |
| `True and None` | ` ` |
| `None and True` | ` ` |
| `False and None` | `False` |
| `None and False` | ` ` |
| `0 and True` | `0` |
| `True and 0` | `0` |
| `0 and False` | `0` |
| `False and 0` | `False` |
| `1 and True` | `True` |
| `True and 1` | `1` |
| `1 and False` | `False` |
| `False and 1` | `False` |
| `"" and True` | `''` |
| `True and ""` | `''` |
| `"" and False` | `''` |
| `False and ""` | `False` |
| `"test" and True` | `True` |
| `True and "test"` | `'test'` |
| `"test" and False` | `False` |
| `False and "test"` | `False` |
| `0 and 1` | `0` |
| `1 and 0` | `0` |
| `2 and 1` | `1` |
| `1 and 2` | `2` |
| `"test" and ""` | `''` |
| `"" and "test"` | `''` |
| `"test1" and "test2"` | `'test2'` |
| `"test2" and "test1"` | `'test1'` |
| `0 and "test"` | `0` |
| `"test" and 0` | `0` |
| `1 and "test"` | `'test'` |
| `"test" and 1` | `1` |

* `@`

| Input | Correct Output |
| ----- | -------------- |
| anything `@` anything | `TypeError` |

* `&`

| Input | Correct Output |
| ----- | -------------- |
| `True & True` | `True` |
| `True & False` | `False` |
| `False & True` | `False` |
| `False & False` | `False` |
| `0 & True` | `0` |
| `True & 0` | `0` |
| `0 & False` | `0` |
| `False & 0` | `0` |
| `1 & True` | `1` |
| `True & 1` | `1` |
| `1 & False` | `0` |
| `False & 1` | `0` |
| `0 & 1` | `0` |
| `1 & 0` | `0` |
| `2 & 1` | `0` |
| `1 & 2` | `0` |
| `2 & 3` | `2` |
| `3 & 2` | `2` |
| `True & None` | `TypeError` |
| `None & True` | `TypeError` |
| `False & None` | `TypeError` |
| `None & False` | `TypeError` |
| `"" & True` | `TypeError` |
| `"test" & True` | `TypeError` |
| `"test" & False` | `TypeError` |
| `"test" & None` | `TypeError` |
| `"test" & 0` | `TypeError` |
| `"test" & "test"` | `TypeError` |

* `~`

| Input | Correct Output |
| ----- | -------------- |
| `~True` | `-2` |
| `~False` | `-1` |
| `~0` | `-1` |
| `~1` | `-2` |
| `~2` | `-3` |
| `~100` | `-101` |
| `~None` | `TypeError` |
| `~""` | `TypeError` |
| `~"test"` | `TypeError` |

* `|`

| Input | Correct Output |
| ----- | -------------- |
| `True \| True` | `True` |
| `True \| False` | `True` |
| `False \| True` | `True` |
| `False \| False` | `False` |
| `0 \| True` | `1` |
| `True \| 0` | `1` |
| `0 \| False` | `0` |
| `False \| 0` | `0` |
| `1 \| True` | `1` |
| `True \| 1` | `1` |
| `1 \| False` | `1` |
| `False \| 1` | `1` |
| `0 \| 1` | `1` |
| `1 \| 0` | `1` |
| `2 \| 1` | `3` |
| `1 \| 2` | `3` |
| `2 \| 3` | `3` |
| `3 \| 2` | `3` |
| `True \| None` | `TypeError` |
| `None \| True` | `TypeError` |
| `False \| None` | `TypeError` |
| `None \| False` | `TypeError` |
| `"" \| True` | `TypeError` |
| `"test" \| True` | `TypeError` |
| `"test" \| False` | `TypeError` |
| `"test" \| None` | `TypeError` |
| `"test" \| 0` | `TypeError` |
| `"test" \| "test"` | `TypeError` |

* `>>`

| Input | Correct Output |
| ----- | -------------- |
| `True >> True` | `0` |
| `True >> False` | `1` |
| `False >> True` | `0` |
| `False >> False` | `0` |
| `0 >> True` | `0` |
| `True >> 0` | `1` |
| `0 >> False` | `0` |
| `False >> 0` | `0` |
| `1 >> True` | `0` |
| `True >> 1` | `0` |
| `1 >> False` | `1` |
| `False >> 1` | `0` |
| `0 >> 1` | `0` |
| `1 >> 0` | `1` |
| `2 >> 1` | `1` |
| `1 >> 2` | `0` |
| `2 >> 3` | `0` |
| `3 >> 2` | `0` |
| `4 >> 1` | `2` |
| `True >> None` | `TypeError` |
| `None >> True` | `TypeError` |
| `False >> None` | `TypeError` |
| `None >> False` | `TypeError` |
| `"" >> True` | `TypeError` |
| `"test" >> True` | `TypeError` |
| `"test" >> False` | `TypeError` |
| `"test" >> None` | `TypeError` |
| `"test" >> 0` | `TypeError` |
| `"test" >> "test"` | `TypeError` |

* `<<`

| Input | Correct Output |
| ----- | -------------- |
| `True << True` | `2` |
| `True << False` | `1` |
| `False << True` | `0` |
| `False << False` | `0` |
| `0 << True` | `0` |
| `True << 0` | `1` |
| `0 << False` | `0` |
| `False << 0` | `0` |
| `1 << True` | `2` |
| `True << 1` | `2` |
| `1 << False` | `1` |
| `False << 1` | `0` |
| `0 << 1` | `0` |
| `1 << 0` | `1` |
| `2 << 1` | `4` |
| `1 << 2` | `4` |
| `2 << 3` | `16` |
| `3 << 2` | `12` |
| `4 << 1` | `8` |
| `True << None` | `TypeError` |
| `None << True` | `TypeError` |
| `False << None` | `TypeError` |
| `None << False` | `TypeError` |
| `"" << True` | `TypeError` |
| `"test" << True` | `TypeError` |
| `"test" << False` | `TypeError` |
| `"test" << None` | `TypeError` |
| `"test" << 0` | `TypeError` |
| `"test" << "test"` | `TypeError` |

* `^`

| Input | Correct Output |
| ----- | -------------- |
| `True ^ True` | `False` |
| `True ^ False` | `True` |
| `False ^ True` | `True` |
| `False ^ False` | `False` |
| `0 ^ True` | `1` |
| `True ^ 0` | `1` |
| `0 ^ False` | `0` |
| `False ^ 0` | `0` |
| `1 ^ True` | `0` |
| `True ^ 1` | `0` |
| `1 ^ False` | `1` |
| `False ^ 1` | `1` |
| `0 ^ 1` | `1` |
| `1 ^ 0` | `1` |
| `2 ^ 1` | `3` |
| `1 ^ 2` | `3` |
| `2 ^ 3` | `1` |
| `3 ^ 2` | `1` |
| `True ^ None` | `TypeError` |
| `None ^ True` | `TypeError` |
| `False ^ None` | `TypeError` |
| `None ^ False` | `TypeError` |
| `"" ^ True` | `TypeError` |
| `"test" ^ True` | `TypeError` |
| `"test" ^ False` | `TypeError` |
| `"test" ^ None` | `TypeError` |
| `"test" ^ 0` | `TypeError` |
| `"test" ^ "test"` | `TypeError` |

* `//`

| Input | Correct Output |
| ----- | -------------- |
| `True // True` | `1` |
| `True // False` | `ZeroDivisionError` |
| `False // True` | `0` |
| `False // False` | `ZeroDivisionError` |
| `0 // True` | `0` |
| `True // 0` | `ZeroDivisionError` |
| `0 // False` | `ZeroDivisionError` |
| `False // 0` | `ZeroDivisionError` |
| `1 // True` | `1` |
| `True // 1` | `1` |
| `1 // False` | `ZeroDivisionError` |
| `False // 1` | `0` |
| `0 // 1` | `0` |
| `1 // 0` | `ZeroDivisionError` |
| `2 // 1` | `2` |
| `1 // 2` | `0` |
| `2 // 3` | `0` |
| `3 // 2` | `1` |
| `100 // 50` | `2` |
| `99 // 50` | `1` |
| `1000 // 12` | `83` |
| `True // None` | `TypeError` |
| `None // True` | `TypeError` |
| `False // None` | `TypeError` |
| `None // False` | `TypeError` |
| `"" // 2` | `TypeError` |
| `"test" // True` | `TypeError` |
| `"test" // False` | `TypeError` |
| `"test" // None` | `TypeError` |
| `"test" // 0` | `TypeError` |
| `"test" // 2` | `TypeError` |
| `"test" // "test"` | `TypeError` |

* `**`

| Input | Correct Output |
| ----- | -------------- |
| `True ** True` | `1` |
| `True ** False` | `1` |
| `False ** True` | `0` |
| `False ** False` | `1` |
| `0 ** True` | `0` |
| `True ** 0` | `1` |
| `0 ** False` | `1` |
| `False ** 0` | `1` |
| `1 ** True` | `1` |
| `True ** 1` | `1` |
| `1 ** False` | `1` |
| `False ** 1` | `0` |
| `0 ** 0` | `1` |
| `0 ** 1` | `0` |
| `1 ** 0` | `1` |
| `2 ** 1` | `2` |
| `1 ** 2` | `1` |
| `2 ** 3` | `8` |
| `3 ** 2` | `9` |
| `True ** None` | `TypeError` |
| `None ** True` | `TypeError` |
| `False ** None` | `TypeError` |
| `None ** False` | `TypeError` |
| `"" ** 2` | `TypeError` |
| `"test" ** True` | `TypeError` |
| `"test" ** False` | `TypeError` |
| `"test" ** None` | `TypeError` |
| `"test" ** 0` | `TypeError` |
| `"test" ** 2` | `TypeError` |
| `"test" ** "test"` | `TypeError` |

* `==`

| Input | Correct Output |
| ----- | -------------- |
| `True == True` | `True` |
| `True == False` | `False` |
| `False == True` | `False` |
| `False == False` | `True` |
| `True == 0` | `False` |
| `0 == True` | `False` |
| `False == 0` | `True` |
| `0 == False` | `True` |
| `True == 1` | `True` |
| `1 == True` | `True` |
| `False == 1` | `False` |
| `1 == False` | `False` |
| `0 == 0` | `True` |
| `0 == 1` | `False` |
| `1 == 0` | `False` |
| `1 == 1` | `True` |
| `50 == 100` | `False` |
| `333 == 333` | `True` |
| `True == None` | `False` |
| `None == True` | `False` |
| `False == None` | `False` |
| `None == False` | `False` |
| `None == None` | `True` |
| `0 == "test"` | `False` |
| `0 == "0"` | `False` |
| `"" == ""` | `True` |
| `"" == "test"` | `False` |
| `"test" == ""` | `False` |
| `"test" == "test"` | `True` |
| `"test1" == "test2"` | `False` |

* `>`

| Input | Correct Output |
| ----- | -------------- |
| `True > True` | `False` |
| `True > False` | `True` |
| `False > True` | `False` |
| `False > False` | `False` |
| `True > 0` | `True` |
| `0 > True` | `False` |
| `False > 0` | `False` |
| `0 > False` | `False` |
| `True > 1` | `False` |
| `1 > True` | `False` |
| `False > 1` | `False` |
| `1 > False` | `True` |
| `0 > 0` | `False` |
| `0 > 1` | `False` |
| `1 > 0` | `True` |
| `1 > 1` | `False` |
| `50 > 100` | `False` |
| `333 > 333` | `False` |
| `87 > 62` | `True` |
| `"" > ""` | `False` |
| `"" > "test"` | `False` |
| `"test" > ""` | `True` |
| `"test" > "test"` | `False` |
| `"test1" > "test2"` | `False` |
| `"test2" > "test1"` | `True` |
| `"test" > 0` | `TypeError` |
| `1 > ""` | `TypeError` |
| `True > None` | `TypeError` |
| `None > True` | `TypeError` |
| `False > None` | `TypeError` |
| `None > False` | `TypeError` |
| `None > None` | `TypeError` |

* `>=`

| Input | Correct Output |
| ----- | -------------- |
| `True >= True` | `True` |
| `True >= False` | `True` |
| `False >= True` | `False` |
| `False >= False` | `True` |
| `True >= 0` | `True` |
| `0 >= True` | `False` |
| `False >= 0` | `True` |
| `0 >= False` | `True` |
| `True >= 1` | `True` |
| `1 >= True` | `True` |
| `False >= 1` | `False` |
| `1 >= False` | `True` |
| `0 >= 0` | `True` |
| `0 >= 1` | `False` |
| `1 >= 0` | `True` |
| `1 >= 1` | `True` |
| `50 >= 100` | `False` |
| `333 >= 333` | `True` |
| `87 >= 62` | `True` |
| `"" >= ""` | `True` |
| `"" >= "test"` | `False` |
| `"test" >= ""` | `True` |
| `"test" >= "test"` | `True` |
| `"test1" >= "test2"` | `False` |
| `"test2" >= "test1"` | `True` |
| `"test" >= 0` | `TypeError` |
| `1 >= ""` | `TypeError` |
| `True >= None` | `TypeError` |
| `None >= True` | `TypeError` |
| `False >= None` | `TypeError` |
| `None >= False` | `TypeError` |
| `None >= None` | `TypeError` |

* `in`

| Input | Correct Output |
| ----- | -------------- |
| `"" in ""` | `True` |
| `"" in "test"` | `True` |
| `"test" in ""` | `False` |
| `"test" in "test"` | `True` |
| `"t" in "test"` | `True` |
| `"te" in "test"` | `True` |
| `"tes" in "test"` | `True` |
| `"e" in "test"` | `True` |
| `"es" in "test"` | `True` |
| `"est" in "test"` | `True` |
| `"s" in "test"` | `True` |
| `"st" in "test"` | `True` |
| `"testing" in "test"` | `False` |
| `"test" in "testing"` | `True` |
| `"test1" in "test2"` | `False` |
| `"test" in True` | `TypeError` |
| `True in "test"` | `TypeError` |
| `"test" in False` | `TypeError` |
| `False in "test"` | `TypeError` |
| `"test" in 0` | `TypeError` |
| `0 in "test"` | `TypeError` |
| `"1" in 1` | `TypeError` |
| `1 in "1"` | `TypeError` |
| `"test" in None` | `TypeError` |
| `None in "test"` | `TypeError` |

* `is`

| Input | Correct Output |
| ----- | -------------- |
| `True is True` | `True` |
| `True is False` | `False` |
| `False is True` | `False` |
| `False is False` | `True` |
| `True is 0` | `False` |
| `0 is True` | `False` |
| `False is 0` | `False` |
| `0 is False` | `False` |
| `True is 1` | `False` |
| `1 is True` | `False` |
| `False is 1` | `False` |
| `1 is False` | `False` |
| `0 is 0` | `True` |
| `0 is 1` | `False` |
| `1 is 0` | `False` |
| `1 is 1` | `True` |
| `50 is 100` | `False` |
| `333 is 333` | `True` |
| `True is None` | `False` |
| `None is True` | `False` |
| `False is None` | `False` |
| `None is False` | `False` |
| `None is None` | `True` |
| `0 is "test"` | `False` |
| `0 is "0"` | `False` |
| `"" is ""` | `True` |
| `"" is "test"` | `False` |
| `"test" is ""` | `False` |
| `"test" is "test"` | `True` |
| `"test1" is "test2"` | `False` |

* `is not`

| Input | Correct Output |
| ----- | -------------- |
| `True is not True` | `False` |
| `True is not False` | `True` |
| `False is not True` | `True` |
| `False is not False` | `False` |
| `True is not 0` | `True` |
| `0 is not True` | `True` |
| `False is not 0` | `True` |
| `0 is not False` | `True` |
| `True is not 1` | `True` |
| `1 is not True` | `True` |
| `False is not 1` | `True` |
| `1 is not False` | `True` |
| `0 is not 0` | `False` |
| `0 is not 1` | `True` |
| `1 is not 0` | `True` |
| `1 is not 1` | `False` |
| `50 is not 100` | `True` |
| `333 is not 333` | `False` |
| `True is not None` | `True` |
| `None is not True` | `True` |
| `False is not None` | `True` |
| `None is not False` | `True` |
| `None is not None` | `False` |
| `0 is not "test"` | `True` |
| `0 is not "0"` | `True` |
| `"" is not ""` | `False` |
| `"" is not "test"` | `True` |
| `"test" is not ""` | `True` |
| `"test" is not "test"` | `False` |
| `"test1" is not "test2"` | `True` |

* `<`

| Input | Correct Output |
| ----- | -------------- |
| `True < True` | `False` |
| `True < False` | `False` |
| `False < True` | `True` |
| `False < False` | `False` |
| `True < 0` | `False` |
| `0 < True` | `True` |
| `False < 0` | `False` |
| `0 < False` | `False` |
| `True < 1` | `False` |
| `1 < True` | `False` |
| `False < 1` | `True` |
| `1 < False` | `False` |
| `0 < 0` | `False` |
| `0 < 1` | `True` |
| `1 < 0` | `False` |
| `1 < 1` | `False` |
| `50 < 100` | `True` |
| `333 < 333` | `False` |
| `87 < 62` | `False` |
| `"" < ""` | `False` |
| `"" < "test"` | `True` |
| `"test" < ""` | `False` |
| `"test" < "test"` | `False` |
| `"test1" < "test2"` | `True` |
| `"test2" < "test1"` | `False` |
| `"test" < 0` | `TypeError` |
| `1 < ""` | `TypeError` |
| `True < None` | `TypeError` |
| `None < True` | `TypeError` |
| `False < None` | `TypeError` |
| `None < False` | `TypeError` |
| `None < None` | `TypeError` |

* `<=`

| Input | Correct Output |
| ----- | -------------- |
| `True <= True` | `True` |
| `True <= False` | `False` |
| `False <= True` | `True` |
| `False <= False` | `True` |
| `True <= 0` | `False` |
| `0 <= True` | `True` |
| `False <= 0` | `True` |
| `0 <= False` | `True` |
| `True <= 1` | `True` |
| `1 <= True` | `True` |
| `False <= 1` | `True` |
| `1 <= False` | `False` |
| `0 <= 0` | `True` |
| `0 <= 1` | `True` |
| `1 <= 0` | `False` |
| `1 <= 1` | `True` |
| `50 <= 100` | `True` |
| `333 <= 333` | `True` |
| `87 <= 62` | `False` |
| `"" <= ""` | `True` |
| `"" <= "test"` | `True` |
| `"test" <= ""` | `False` |
| `"test" <= "test"` | `True` |
| `"test1" <= "test2"` | `True` |
| `"test2" <= "test1"` | `False` |
| `"test" <= 0` | `TypeError` |
| `1 <= ""` | `TypeError` |
| `True <= None` | `TypeError` |
| `None <= True` | `TypeError` |
| `False <= None` | `TypeError` |
| `None <= False` | `TypeError` |
| `None <= None` | `TypeError` |

* `-` (binary)

| Input | Correct Output |
| ----- | -------------- |
| `True - True` | `0` |
| `True - False` | `1` |
| `False - True` | `-1` |
| `False - False` | `0` |
| `0 - True` | `-1` |
| `True - 0` | `1` |
| `0 - False` | `0` |
| `False - 0` | `0` |
| `1 - True` | `0` |
| `True - 1` | `0` |
| `1 - False` | `1` |
| `False - 1` | `-1` |
| `0 - 1` | `-1` |
| `1 - 0` | `1` |
| `2 - 1` | `1` |
| `1 - 2` | `-1` |
| `2 - 3` | `-1` |
| `3 - 2` | `1` |
| `100 - 50` | `50` |
| `99 - 50` | `49` |
| `1000 - 12` | `988` |
| `True - None` | `TypeError` |
| `None - True` | `TypeError` |
| `False - None` | `TypeError` |
| `None - False` | `TypeError` |
| `"" - 2` | `TypeError` |
| `"test" - True` | `TypeError` |
| `"test" - False` | `TypeError` |
| `"test" - None` | `TypeError` |
| `"test" - 0` | `TypeError` |
| `"test" - 2` | `TypeError` |
| `"test" - "test"` | `TypeError` |

* `-` (unary)

| Input | Correct Output |
| ----- | -------------- |
| `-True` | `-1` |
| `-False` | `0` |
| `-0` | `0` |
| `-1` | `-1` |
| `-2` | `-2` |
| `-100` | `-100` |
| `-None` | `TypeError` |
| `-""` | `TypeError` |
| `-"test"` | `TypeError` |

* `!=`

| Input | Correct Output |
| ----- | -------------- |
| `True != True` | `False` |
| `True != False` | `True` |
| `False != True` | `True` |
| `False != False` | `False` |
| `True != 0` | `True` |
| `0 != True` | `True` |
| `False != 0` | `False` |
| `0 != False` | `False` |
| `True != 1` | `False` |
| `1 != True` | `False` |
| `False != 1` | `True` |
| `1 != False` | `True` |
| `0 != 0` | `False` |
| `0 != 1` | `True` |
| `1 != 0` | `True` |
| `1 != 1` | `False` |
| `50 != 100` | `True` |
| `333 != 333` | `False` |
| `True != None` | `True` |
| `None != True` | `True` |
| `False != None` | `True` |
| `None != False` | `True` |
| `None != None` | `False` |
| `0 != "test"` | `True` |
| `0 != "0"` | `True` |
| `"" != ""` | `False` |
| `"" != "test"` | `True` |
| `"test" != ""` | `True` |
| `"test" != "test"` | `False` |
| `"test1" != "test2"` | `True` |

* `not in`

| Input | Correct Output |
| ----- | -------------- |
| `"" not in ""` | `False` |
| `"" not in "test"` | `False` |
| `"test" not in ""` | `True` |
| `"test" not in "test"` | `False` |
| `"t" not in "test"` | `False` |
| `"te" not in "test"` | `False` |
| `"tes" not in "test"` | `False` |
| `"e" not in "test"` | `False` |
| `"es" not in "test"` | `False` |
| `"est" not in "test"` | `False` |
| `"s" not in "test"` | `False` |
| `"st" not in "test"` | `False` |
| `"testing" not in "test"` | `True` |
| `"test" not in "testing"` | `False` |
| `"test1" not in "test2"` | `True` |
| `"test" not in True` | `TypeError` |
| `True not in "test"` | `TypeError` |
| `"test" not in False` | `TypeError` |
| `False not in "test"` | `TypeError` |
| `"test" not in 0` | `TypeError` |
| `0 not in "test"` | `TypeError` |
| `"1" not in 1` | `TypeError` |
| `1 not in "1"` | `TypeError` |
| `"test" not in None` | `TypeError` |
| `None not in "test"` | `TypeError` |

* `or`

| Input | Correct Output |
| ----- | -------------- |
| `True or True` | `True` |
| `True or False` | `True` |
| `False or True` | `True` |
| `False or False` | `False` |
| `True or None` | `True` |
| `None or True` | `True` |
| `False or None` | ` ` |
| `None or False` | `False` |
| `0 or True` | `True` |
| `True or 0` | `True` |
| `0 or False` | `False` |
| `False or 0` | `0` |
| `1 or True` | `1` |
| `True or 1` | `True` |
| `1 or False` | `1` |
| `False or 1` | `1` |
| `"" or True` | `True` |
| `True or ""` | `True` |
| `"" or False` | `False` |
| `False or ""` | `''` |
| `"test" or True` | `'test'` |
| `True or "test"` | `True` |
| `"test" or False` | `'test'` |
| `False or "test"` | `'test'` |
| `0 or 1` | `1` |
| `1 or 0` | `1` |
| `2 or 1` | `2` |
| `1 or 2` | `1` |
| `"test" or ""` | `'test'` |
| `"" or "test"` | `'test'` |
| `"test1" or "test2"` | `'test1'` |
| `"test2" or "test1"` | `'test2'` |
| `0 or "test"` | `'test'` |
| `"test" or 0` | `'test'` |
| `1 or "test"` | `1` |
| `"test" or 1` | `'test'` |

* `%`

| Input | Correct Output |
| ----- | -------------- |
| `True % True` | `0` |
| `True % False` | `ZeroDivisionError` |
| `False % True` | `0` |
| `False % False` | `ZeroDivisionError` |
| `0 % True` | `0` |
| `True % 0` | `ZeroDivisionError` |
| `0 % False` | `ZeroDivisionError` |
| `False % 0` | `ZeroDivisionError` |
| `1 % True` | `0` |
| `True % 1` | `0` |
| `1 % False` | `ZeroDivisionError` |
| `False % 1` | `0` |
| `0 % 1` | `0` |
| `1 % 0` | `ZeroDivisionError` |
| `2 % 1` | `0` |
| `1 % 2` | `1` |
| `2 % 3` | `2` |
| `3 % 2` | `1` |
| `100 % 50` | `0` |
| `99 % 50` | `49` |
| `1000 % 12` | `4` |
| `True % None` | `TypeError` |
| `None % True` | `TypeError` |
| `False % None` | `TypeError` |
| `None % False` | `TypeError` |
| `"" % 2` | `TypeError` |
| `"test" % True` | `TypeError` |
| `"test" % False` | `TypeError` |
| `"test" % None` | `TypeError` |
| `"test" % 0` | `TypeError` |
| `"test" % 2` | `TypeError` |
| `"test" % "test"` | `TypeError` |

* `+` (binary)

| Input | Correct Output |
| ----- | -------------- |
| `True + True` | `2` |
| `True + False` | `1` |
| `False + True` | `1` |
| `False + False` | `0` |
| `0 + True` | `1` |
| `True + 0` | `1` |
| `0 + False` | `0` |
| `False + 0` | `0` |
| `1 + True` | `2` |
| `True + 1` | `2` |
| `1 + False` | `1` |
| `False + 1` | `1` |
| `0 + 1` | `1` |
| `1 + 0` | `1` |
| `2 + 1` | `3` |
| `1 + 2` | `3` |
| `2 + 3` | `5` |
| `3 + 2` | `5` |
| `100 + 50` | `150` |
| `99 + 50` | `149` |
| `1000 + 12` | `1012` |
| `"" + ""` | `''` |
| `"" + "test"` | `'test'` |
| `"test" + ""` | `'test'` |
| `"test" + "test"` | `'testtest'` |
| `"test1" + "test2"` | `'test1test2'` |
| `True + None` | `TypeError` |
| `None + True` | `TypeError` |
| `False + None` | `TypeError` |
| `None + False` | `TypeError` |
| `"" + 2` | `TypeError` |
| `"test" + True` | `TypeError` |
| `"test" + False` | `TypeError` |
| `"test" + None` | `TypeError` |
| `"test" + 0` | `TypeError` |
| `"test" + 2` | `TypeError` |

* `+` (unary)

| Input | Correct Output |
| ----- | -------------- |
| `+True` | `1` |
| `+False` | `0` |
| `+0` | `0` |
| `+1` | `1` |
| `+2` | `2` |
| `+100` | `100` |
| `+None` | `TypeError` |
| `+""` | `TypeError` |
| `+"test"` | `TypeError` |

* `/`

Note that in CPython, floating-point numbers are returned from this operation. However, the C3 does not have floating-point hardware, so this operator implements integer division (making the desired results almost identical to the results for the `//` operator).

| Input | Correct Output |
| ----- | -------------- |
| `True / True` | `1` (really `1.0`) |
| `True / False` | `ZeroDivisionError` |
| `False / True` | `0` (really `0.0`) |
| `False / False` | `ZeroDivisionError` |
| `0 / True` | `0` (really `0.0`)|
| `True / 0` | `ZeroDivisionError` |
| `0 / False` | `ZeroDivisionError` |
| `False / 0` | `ZeroDivisionError` |
| `1 / True` | `1` (really `1.0`) |
| `True / 1` | `1` (really `1.0`) |
| `1 / False` | `ZeroDivisionError` |
| `False / 1` | `0` (really `0.0`) |
| `0 / 1` | `0` (really `0.0`) |
| `1 / 0` | `ZeroDivisionError` |
| `2 / 1` | `2` (really `2.0`) |
| `1 / 2` | `0` (really `0.5`) |
| `2 / 3` | `0` (really `0.6666666666666666`) |
| `3 / 2` | `1` (really `1.5`) |
| `100 / 50` | `2` (really `2.0`) |
| `99 / 50` | `1` (really `1.98`) |
| `1000 / 12` | `83` (really `83.33333333333333`) |
| `True / None` | `TypeError` |
| `None / True` | `TypeError` |
| `False / None` | `TypeError` |
| `None / False` | `TypeError` |
| `"" / 2` | `TypeError` |
| `"test" / True` | `TypeError` |
| `"test" / False` | `TypeError` |
| `"test" / None` | `TypeError` |
| `"test" / 0` | `TypeError` |
| `"test" / 2` | `TypeError` |
| `"test" / "test"` | `TypeError` |

* `*`

| Input | Correct Output |
| ----- | -------------- |
| `True * True` | `1` |
| `True * False` | `0` |
| `False * True` | `0` |
| `False * False` | `0` |
| `0 * True` | `0` |
| `True * 0` | `0` |
| `0 * False` | `0` |
| `False * 0` | `0` |
| `1 * True` | `1` |
| `True * 1` | `1` |
| `1 * False` | `0` |
| `False * 1` | `0` |
| `0 * 1` | `0` |
| `1 * 0` | `0` |
| `2 * 1` | `2` |
| `1 * 2` | `2` |
| `2 * 3` | `6` |
| `3 * 2` | `6` |
| `100 * 50` | `5000` |
| `99 * 50` | `4950` |
| `1000 * 12` | `12000` |
| `"" * 2` | `''` |
| `"test" * True` | `'test'` |
| `"test" * False` | `''` |
| `"test" * 0` | `''` |
| `0 * "test"` | `''` |
| `"test" * 1` | `'test'` |
| `1 * "test"` | `'test'` |
| `"test" * 2` | `'testtest'` |
| `2 * "test"` | `'testtest'` |
| `"test" * "test"` | `TypeError` |
| `True * None` | `TypeError` |
| `None * True` | `TypeError` |
| `False * None` | `TypeError` |
| `None * False` | `TypeError` |
| `1 * None` | `TypeError` |
| `None * 1` | `TypeError` |
| `"test" * None` | `TypeError` |

### Integration Tests

* arithmetic

| Input | Correct Output |
| ----- | -------------- |
| `--1` | `1` |
| `++1` | `1` |
| `+-1` | `-1` |
| `-+1` | `-1` |
| `1 + 2 + 3 + 4 + 5` | `15` |
| `1 - 2 - 3 - 4 - 5` | `-13` |
| `+1 + +2 + +3 + +4 + +5` | `15` |
| `+1 - +2 - +3 - +4 - +5` | `-13` |
| `-1 + -2 + -3 + -4 + -5` | `-15` |
| `-1 - -2 - -3 - -4 - -5` | `13` |
| `18 * 9 / 9` | `18` (really `18.0`) |
| `18 / 9 * 9` | `18` (really `18.0`) |
| `8 * 9 // 9` | `8` |
| `8 // 9 * 9` | `0` |
| `2 * -3` | `-6` |
| `-2 * 3` | `-6` |
| `-2 * -3` | `6` |
| `76 // -3` | `-26` |
| `-76 // 3` | `-26` |
| `-76 // -3` | `25` |
| `4 * 3 + 2 * 9` | `30` |
| `4 * (3 + 2) * 9` | `180` |
| `4 * 3 - 2 * 9` | `-6` |
| `4 * (3 - 2) * 9` | `36` |
| `4 * (3 - 2) * 9` | `36` |
| `2 ** 3 ** 2` | `512` |
| `(2 ** 3) ** 2` | `64` |
| `-1 ** 2` | `-1` |
| `(-1) ** 2` | `1` |
| `5 ** -1` | `0` (really `0.2`) |
| `5 ** (-1)` | `0` (really `0.2`) |
| `12 // 4 - 2 ** 3 + 6 - 7` | `-6` |
| `12 // (4 - 2) ** 3 + 6 - 7` | `0` |
| `(12 // 4 - 2) ** (3 + 6 - 7)` | `1` |
| `16 % 5 % 3` | `1` |
| `16 % (5 % 3)` | `0` |
| `32 % -5` | `-3` |
| `-32 % 5` | `3` |
| `-32 % -5` | `-2` |
| `2 ** 4 % 7 + 5 * -2 - 17 // 3` | `-13` |
| `2 ** (4 % 7 + 5) * -2 - 17 // 3` | `-1029` |
| `2 ** 4 % 7 + 5 * (-2 - 17) // 3` | `-30` |

* bitwise

| Input | Correct Output |
| ----- | -------------- |
| `~~11` | `11` |
| `5 << 1 << 1` | `20` |
| `5 >> 1 >> 1` | `1` |
| `5 << 1 >> 1` | `5` |
| `5 >> 1 << 1` | `4` |
| `8 & 4 & 2 & 1` | `0` |
| `15 & 7 & 3 & 1` | `1` |
| `8 \| 4 \| 2 \| 1` | `15` |
| `15 \| 7 \| 3 \| 1` | `15` |
| `8 ^ 4 ^ 2 ^ 1` | `15` |
| `15 ^ 7 ^ 3 ^ 1` | `10` |
| `13 & 6 ^ 9 \| 11` | `15` |
| `13 & 6 \| 9 ^ 11` | `6` |
| `13 ^ 6 & 9 \| 11` | `15` |
| `13 ^ 6 \| 9 & 11` | `11` |
| `13 \| 6 & 9 ^ 11` | `15` |
| `13 \| 6 ^ 9 & 11` | `15` |
| `~13 & ~6 ^ ~9 \| ~11` | `-10` |
| `~13 & ~6 \| ~9 ^ ~11` | `-14` |
| `~13 ^ ~6 & ~9 \| ~11` | `-10` |
| `~13 ^ ~6 \| ~9 & ~11` | `-1` |
| `~13 \| ~6 & ~9 ^ ~11` | `-10` |
| `~13 \| ~6 ^ ~9 & ~11` | `-1` |
| `8 \| 23 >> 3 & 5` | `8` |
| `(8 \| 23) >> (3 & 5)` | `15` |
| `21 & 7 >> 6 ^ 10` | `10` |
| `(21 & 7) >> (6 ^ 10)` | `0` |
| `8 \| 23 << 3 & 5` | `8` |
| `(8 \| 23) << (3 & 5)` | `62` |
| `21 & 7 << 6 ^ 10` | `10` |
| `(21 & 7) << (6 ^ 10)` | `20480` |
| `~18 \| 39 << 2 ^ 5 >> 9` | `-3` |
| `~(18 \| 39 << 2 ^ 5 >> 9)` | `-159` |

* comparison

| Input | Correct Output |
| ----- | -------------- |
| `False == 0 == False` | `True` |
| `(False == 0) == False` | `False` |
| `True == 0 == False` | `False` |
| `(True == 0) == False` | `True` |
| `False == 0 == True` | `False` |
| `(False == 0) == True` | `True` |
| `True == 0 == True` | `False` |
| `(True == 0) == True` | `False` |
| `(True == 1) is True` | `True` |
| `(True == 1) is 1` | `False` |
| `(True is 1) == (True == 1)` | `False` |
| `(False is 0) == (False == 0)` | `False` |
| `(True is 1) == (False is 0)` | `True` |
| `(True == 1) is (False == 0)` | `True` |
| `(True is 1) is (False is 0)` | `True` |
| `(True == 1) is not True` | `False` |
| `(True == 1) is not 1` | `True` |
| `(True is not 1) == (True == 1)` | `True` |
| `(False is not 0) == (False == 0)` | `True` |
| `(True is not 1) == (False is not 0)` | `True` |
| `(True == 1) is not (False == 0)` | `False` |
| `(True is not 1) is not (False is not 0)` | `False` |
| `2 != 0 != 1` | `True` |
| `(2 != 0) and (0 != 1)` | `True` |
| `(2 != 0) != 1` | `False` |
| `2 != (0 != 1)` | `True` |
| `1 != 2 != 0` | `True` |
| `(1 != 2) and (2 != 0)` | `True` |
| `(1 != 2) != 0` | `True` |
| `1 != (2 != 0)` | `False` |
| `-1 < 0 < 1` | `True` |
| `(-1 < 0) and (0 < 1)` | `True` |
| `(-1 < 0) < 1` | `False` |
| `-1 < (0 < 1)` | `True` |
| `-1 <= 0 <= 1` | `True` |
| `(-1 <= 0) and (0 <= 1)` | `True` |
| `(-1 <= 0) <= 1` | `True` |
| `-1 <= (0 <= 1)` | `True` |
| `1 > 0 > -1` | `True` |
| `(1 > 0) and (0 > -1)` | `True` |
| `(1 > 0) > -1` | `True` |
| `1 > (0 > -1)` | `False` |
| `1 >= 0 >= -1` | `True` |
| `(1 >= 0) and (0 >= -1)` | `True` |
| `(1 >= 0) >= -1` | `True` |
| `1 >= (0 >= -1)` | `True` |
| `(0 > 10) == (0 >= 10)` | `True` |
| `(10 > 10) == (10 >= 10)` | `False` |
| `(20 > 10) == (20 >= 10)` | `True` |
| `(0 < 10) == (0 <= 10)` | `True` |
| `(10 < 10) == (10 <= 10)` | `False` |
| `(20 < 10) == (20 <= 10)` | `True` |

* logical

| Input | Correct Output |
| ----- | -------------- |
| `False and False and False` | `False` |
| `True and True and False` | `False` |
| `True and True and True` | `True` |
| `not False and not False and not False` | `True` |
| `False and False and not False` | `False` |
| `not True and True and True` | `False` |
| `not not True and True and True` | `True` |
| `not (not True and True and True)` | `True` |
| `not (not False and True and True)` | `False` |
| `False or False or False` | `False` |
| `True or True or False` | `True` |
| `True or True or True` | `True` |
| `not False or not False or not False` | `True` |
| `False or False or not False` | `True` |
| `not True or True or True` | `True` |
| `not not True or True or True` | `True` |
| `not (not True or True or True)` | `False` |
| `not (not False or True or True)` | `False` |
| `True and True or True and True` | `True` |
| `False and True or True and True` | `True` |
| `False and False or True and True` | `True` |
| `False and False or False and True` | `False` |
| `True or False or True and False` | `True` |
| `False or False or True and False` | `False` |
| `(True or False) and (True or False)` | `True` |
| `(True or False) and (False or False)` | `False` |

* string

| Input | Correct Output |
| ----- | -------------- |
| `"" + "test" * 2` | `'testtest'` |
| `"test" + "" * 2` | `'test'` |
| `("" + "test") * 2` | `'testtest'` |
| `("test" + "") * 2` | `'testtest'` |
| `"test" + "test" * 2` | `'testtesttest'` |
| `("test" + "test") * 2` | `'testtesttesttest'` |
| `"" * 10 in ""` | `True` |
| `"" in "" * 10` | `True` |
| `"test" * 1 in "test" * 1` | `True` |
| `"test" * 1 in "test" * 2` | `True` |
| `"test" * 2 in "test" * 1` | `False` |
| `"test" * 2 in "test" * 2` | `True` |
| `"test" * 3 in "test" * 2` | `False` |
| `"test" * 2 in "test" * 3` | `True` |
| `"" * 10 not in ""` | `False` |
| `"" not in "" * 10` | `False` |
| `"test" * 1 not in "test" * 1` | `False` |
| `"test" * 1 not in "test" * 2` | `False` |
| `"test" * 2 not in "test" * 1` | `True` |
| `"test" * 2 not in "test" * 2` | `False` |
| `"test" * 3 not in "test" * 2` | `True` |
| `"test" * 2 not in "test" * 3` | `False` |
