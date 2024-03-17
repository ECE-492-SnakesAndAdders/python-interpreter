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

Note that in CPython, floating-point numbers are returned from this operation. However, the C3 does not have floating-point hardware, so this operator implements integer division (making the desired results identical to the results for `//`).

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

* arithmetic

* string
