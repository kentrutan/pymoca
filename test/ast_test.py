import inspect

from pymoca import ast

ast_tree = ast.Tree()


def test_add_class():
    c = ast.Class(name="TestClass")
    ast_tree.add_class(c)

    assert c.parent == ast_tree
    assert c.name in ast_tree.classes

    ast_tree.remove_class(c)
    assert c.name not in ast_tree.classes


def test_add_symbol():
    s = ast.Symbol(name="TestSymbol", type=ast.ComponentRef.from_tuple("Real"))
    ast_tree.add_symbol(s)

    assert s.name in ast_tree.symbols

    ast_tree.remove_symbol(s)
    assert s.name not in ast_tree.symbols


def test_add_equation():
    e = ast.Equation(left=ast.ComponentRef.from_tuple("a"), right=ast.ComponentRef.from_tuple("b"))
    ast_tree.add_equation(e)

    assert e in ast_tree.equations

    ast_tree.remove_equation(e)
    assert e not in ast_tree.equations

    ast_tree.add_initial_equation(e)
    assert e in ast_tree.initial_equations

    ast_tree.remove_initial_equation(e)
    assert e not in ast_tree.initial_equations


def test_all_repr_and_str_len():
    for attr_string in dir(ast):
        attr = getattr(ast, attr_string)
        if inspect.isclass(attr) and issubclass(attr, ast.Node):
            class_instance = attr()
            assert len(repr(class_instance)) != 0
            if isinstance(class_instance, ast.ComponentRef):
                assert len(str(class_instance)) == 0
            else:
                assert len(str(class_instance)) != 0


def test_component_ref():
    cref = ast.ComponentRef.from_string("A0.B1.C2")

    cref_tuple = cref.to_tuple()
    assert cref_tuple[0] == "A0"
    assert cref_tuple[1] == "B1"
    assert cref_tuple[2] == "C2"

    cref_d3 = ast.ComponentRef.from_string("D3")
    assert str(cref_d3) == "D3"

    cref_cat = cref.from_tuple(cref_tuple + cref_d3.to_tuple())
    assert str(cref_cat) == "A0.B1.C2.D3"
    assert repr(cref_cat) == "'A0'['B1'['C2'['D3']]]"
