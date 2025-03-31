// Test classes and components with same name at different levels of the tree
package Tree
    constant Integer t = 0;
    model SubTree
        Integer t = 1;
    end SubTree;
    model Tree
        extends SubTree(t = 2);
    equation
        t = 3;
    end Tree;
end Tree;
