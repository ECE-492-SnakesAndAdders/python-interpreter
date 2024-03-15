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
| `True and None` | `` |
| `None and True` | `` |
| `False and None` | `False` |
| `None and False` | `` |
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
| ` ` | ` ` |

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
| ` ` | ` ` |

* `<<`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

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
| ` ` | ` ` |

* `**`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `==`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `>`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `>=`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `is`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `<`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `<=`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `-`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `!=`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `or`

| Input | Correct Output |
| ----- | -------------- |
| `True or True` | `True` |
| `True or False` | `True` |
| `False or True` | `True` |
| `False or False` | `False` |
| `True or None` | `True` |
| `None or True` | `True` |
| `False or None` | `` |
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
| ` ` | ` ` |

* `+`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `/`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

* `*`

| Input | Correct Output |
| ----- | -------------- |
| ` ` | ` ` |

### Integration Tests
