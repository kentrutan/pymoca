from pymoca.backends.casadi.alias_relation import AliasRelation


def test_double_alias():
    alias_relation = AliasRelation()

    alias_relation.add("a", "b")
    alias_relation.add("b", "c")
    alias_relation.add("c", "a")

    assert alias_relation.canonical_variables == {
        "a",
    }
    assert alias_relation.aliases("a") == {"a", "b", "c"}
