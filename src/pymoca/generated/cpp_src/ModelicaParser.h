
// Generated from Modelica.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  ModelicaParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    T__38 = 39, T__39 = 40, T__40 = 41, T__41 = 42, T__42 = 43, T__43 = 44, 
    T__44 = 45, T__45 = 46, T__46 = 47, T__47 = 48, T__48 = 49, T__49 = 50, 
    T__50 = 51, T__51 = 52, T__52 = 53, T__53 = 54, T__54 = 55, T__55 = 56, 
    T__56 = 57, T__57 = 58, T__58 = 59, T__59 = 60, T__60 = 61, T__61 = 62, 
    T__62 = 63, T__63 = 64, T__64 = 65, T__65 = 66, T__66 = 67, T__67 = 68, 
    T__68 = 69, T__69 = 70, T__70 = 71, T__71 = 72, T__72 = 73, T__73 = 74, 
    T__74 = 75, T__75 = 76, T__76 = 77, T__77 = 78, T__78 = 79, EACH = 80, 
    PARTIAL = 81, FINAL = 82, WITHIN = 83, ENCAPSULATED = 84, REDECLARE = 85, 
    INNER = 86, OUTER = 87, INITIAL = 88, IDENT = 89, STRING = 90, UNSIGNED_NUMBER = 91, 
    COMMENT = 92, WS = 93
  };

  enum {
    RuleStored_definition = 0, RuleStored_definition_class = 1, RuleClass_definition = 2, 
    RuleClass_prefixes = 3, RuleClass_type = 4, RuleClass_specifier = 5, 
    RuleBase_prefix = 6, RuleEnum_list = 7, RuleEnumeration_literal = 8, 
    RuleComposition = 9, RuleLanguage_specification = 10, RuleExternal_function_call = 11, 
    RuleElement_list = 12, RuleElement = 13, RuleRegular_element = 14, RuleReplaceable_element = 15, 
    RuleImport_clause = 16, RuleImport_list = 17, RuleExtends_clause = 18, 
    RuleConstraining_clause = 19, RuleComponent_clause = 20, RuleType_prefix = 21, 
    RuleType_specifier_element = 22, RuleType_specifier = 23, RuleComponent_list = 24, 
    RuleComponent_declaration = 25, RuleCondition_attribute = 26, RuleDeclaration = 27, 
    RuleModification = 28, RuleClass_modification = 29, RuleArgument_list = 30, 
    RuleArgument = 31, RuleElement_modification_or_replaceable = 32, RuleElement_modification = 33, 
    RuleElement_redeclaration = 34, RuleElement_replaceable = 35, RuleComponent_clause1 = 36, 
    RuleComponent_declaration1 = 37, RuleShort_class_definition = 38, RuleEquation_block = 39, 
    RuleEquation_section = 40, RuleStatement_block = 41, RuleAlgorithm_section = 42, 
    RuleEquation_options = 43, RuleEquation = 44, RuleStatement_options = 45, 
    RuleStatement = 46, RuleIf_equation = 47, RuleIf_statement = 48, RuleFor_equation = 49, 
    RuleFor_statement = 50, RuleFor_indices = 51, RuleFor_index = 52, RuleWhile_statement = 53, 
    RuleWhen_equation = 54, RuleWhen_statement = 55, RuleConnect_clause = 56, 
    RuleExpression = 57, RuleSimple_expression = 58, RuleExpr = 59, RulePrimary = 60, 
    RuleName = 61, RuleComponent_reference_element = 62, RuleComponent_reference = 63, 
    RuleFunction_call_args = 64, RuleFunction_arguments = 65, RuleNamed_arguments = 66, 
    RuleNamed_argument = 67, RuleFunction_argument = 68, RuleOutput_expression_list = 69, 
    RuleExpression_list = 70, RuleArray_subscripts = 71, RuleSubscript = 72, 
    RuleComment = 73, RuleString_comment = 74, RuleAnnotation = 75
  };

  explicit ModelicaParser(antlr4::TokenStream *input);
  ~ModelicaParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class Stored_definitionContext;
  class Stored_definition_classContext;
  class Class_definitionContext;
  class Class_prefixesContext;
  class Class_typeContext;
  class Class_specifierContext;
  class Base_prefixContext;
  class Enum_listContext;
  class Enumeration_literalContext;
  class CompositionContext;
  class Language_specificationContext;
  class External_function_callContext;
  class Element_listContext;
  class ElementContext;
  class Regular_elementContext;
  class Replaceable_elementContext;
  class Import_clauseContext;
  class Import_listContext;
  class Extends_clauseContext;
  class Constraining_clauseContext;
  class Component_clauseContext;
  class Type_prefixContext;
  class Type_specifier_elementContext;
  class Type_specifierContext;
  class Component_listContext;
  class Component_declarationContext;
  class Condition_attributeContext;
  class DeclarationContext;
  class ModificationContext;
  class Class_modificationContext;
  class Argument_listContext;
  class ArgumentContext;
  class Element_modification_or_replaceableContext;
  class Element_modificationContext;
  class Element_redeclarationContext;
  class Element_replaceableContext;
  class Component_clause1Context;
  class Component_declaration1Context;
  class Short_class_definitionContext;
  class Equation_blockContext;
  class Equation_sectionContext;
  class Statement_blockContext;
  class Algorithm_sectionContext;
  class Equation_optionsContext;
  class EquationContext;
  class Statement_optionsContext;
  class StatementContext;
  class If_equationContext;
  class If_statementContext;
  class For_equationContext;
  class For_statementContext;
  class For_indicesContext;
  class For_indexContext;
  class While_statementContext;
  class When_equationContext;
  class When_statementContext;
  class Connect_clauseContext;
  class ExpressionContext;
  class Simple_expressionContext;
  class ExprContext;
  class PrimaryContext;
  class NameContext;
  class Component_reference_elementContext;
  class Component_referenceContext;
  class Function_call_argsContext;
  class Function_argumentsContext;
  class Named_argumentsContext;
  class Named_argumentContext;
  class Function_argumentContext;
  class Output_expression_listContext;
  class Expression_listContext;
  class Array_subscriptsContext;
  class SubscriptContext;
  class CommentContext;
  class String_commentContext;
  class AnnotationContext; 

  class  Stored_definitionContext : public antlr4::ParserRuleContext {
  public:
    Stored_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITHIN();
    std::vector<Stored_definition_classContext *> stored_definition_class();
    Stored_definition_classContext* stored_definition_class(size_t i);
    Component_referenceContext *component_reference();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stored_definitionContext* stored_definition();

  class  Stored_definition_classContext : public antlr4::ParserRuleContext {
  public:
    Stored_definition_classContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Class_definitionContext *class_definition();
    antlr4::tree::TerminalNode *FINAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stored_definition_classContext* stored_definition_class();

  class  Class_definitionContext : public antlr4::ParserRuleContext {
  public:
    Class_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Class_prefixesContext *class_prefixes();
    Class_specifierContext *class_specifier();
    antlr4::tree::TerminalNode *ENCAPSULATED();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Class_definitionContext* class_definition();

  class  Class_prefixesContext : public antlr4::ParserRuleContext {
  public:
    Class_prefixesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Class_typeContext *class_type();
    antlr4::tree::TerminalNode *PARTIAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Class_prefixesContext* class_prefixes();

  class  Class_typeContext : public antlr4::ParserRuleContext {
  public:
    Class_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Class_typeContext* class_type();

  class  Class_specifierContext : public antlr4::ParserRuleContext {
  public:
    Class_specifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Class_specifierContext() = default;
    void copyFrom(Class_specifierContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Class_spec_derContext : public Class_specifierContext {
  public:
    Class_spec_derContext(Class_specifierContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> IDENT();
    antlr4::tree::TerminalNode* IDENT(size_t i);
    NameContext *name();
    CommentContext *comment();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Class_spec_enumContext : public Class_specifierContext {
  public:
    Class_spec_enumContext(Class_specifierContext *ctx);

    antlr4::tree::TerminalNode *IDENT();
    CommentContext *comment();
    Enum_listContext *enum_list();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Class_spec_baseContext : public Class_specifierContext {
  public:
    Class_spec_baseContext(Class_specifierContext *ctx);

    antlr4::tree::TerminalNode *IDENT();
    Base_prefixContext *base_prefix();
    Component_referenceContext *component_reference();
    CommentContext *comment();
    Class_modificationContext *class_modification();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Class_spec_compContext : public Class_specifierContext {
  public:
    Class_spec_compContext(Class_specifierContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> IDENT();
    antlr4::tree::TerminalNode* IDENT(size_t i);
    String_commentContext *string_comment();
    CompositionContext *composition();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Class_spec_extendsContext : public Class_specifierContext {
  public:
    Class_spec_extendsContext(Class_specifierContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> IDENT();
    antlr4::tree::TerminalNode* IDENT(size_t i);
    String_commentContext *string_comment();
    CompositionContext *composition();
    Class_modificationContext *class_modification();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Class_specifierContext* class_specifier();

  class  Base_prefixContext : public antlr4::ParserRuleContext {
  public:
    Base_prefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Type_prefixContext *type_prefix();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Base_prefixContext* base_prefix();

  class  Enum_listContext : public antlr4::ParserRuleContext {
  public:
    Enum_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Enumeration_literalContext *> enumeration_literal();
    Enumeration_literalContext* enumeration_literal(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Enum_listContext* enum_list();

  class  Enumeration_literalContext : public antlr4::ParserRuleContext {
  public:
    Enumeration_literalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();
    CommentContext *comment();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Enumeration_literalContext* enumeration_literal();

  class  CompositionContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::Element_listContext *epriv = nullptr;
    ModelicaParser::Element_listContext *epub = nullptr;
    ModelicaParser::Element_listContext *epro = nullptr;
    ModelicaParser::AnnotationContext *ext_annotation = nullptr;
    ModelicaParser::AnnotationContext *comp_annotation = nullptr;
    CompositionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Element_listContext *> element_list();
    Element_listContext* element_list(size_t i);
    std::vector<Equation_sectionContext *> equation_section();
    Equation_sectionContext* equation_section(size_t i);
    std::vector<Algorithm_sectionContext *> algorithm_section();
    Algorithm_sectionContext* algorithm_section(size_t i);
    std::vector<AnnotationContext *> annotation();
    AnnotationContext* annotation(size_t i);
    Language_specificationContext *language_specification();
    External_function_callContext *external_function_call();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompositionContext* composition();

  class  Language_specificationContext : public antlr4::ParserRuleContext {
  public:
    Language_specificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Language_specificationContext* language_specification();

  class  External_function_callContext : public antlr4::ParserRuleContext {
  public:
    External_function_callContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();
    Component_referenceContext *component_reference();
    Expression_listContext *expression_list();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  External_function_callContext* external_function_call();

  class  Element_listContext : public antlr4::ParserRuleContext {
  public:
    Element_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ElementContext *> element();
    ElementContext* element(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Element_listContext* element_list();

  class  ElementContext : public antlr4::ParserRuleContext {
  public:
    ElementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Import_clauseContext *import_clause();
    Extends_clauseContext *extends_clause();
    Regular_elementContext *regular_element();
    Replaceable_elementContext *replaceable_element();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementContext* element();

  class  Regular_elementContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::Class_definitionContext *class_elem = nullptr;
    ModelicaParser::Component_clauseContext *comp_elem = nullptr;
    Regular_elementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REDECLARE();
    antlr4::tree::TerminalNode *FINAL();
    antlr4::tree::TerminalNode *INNER();
    antlr4::tree::TerminalNode *OUTER();
    Class_definitionContext *class_definition();
    Component_clauseContext *component_clause();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Regular_elementContext* regular_element();

  class  Replaceable_elementContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::Class_definitionContext *class_elem = nullptr;
    ModelicaParser::Component_clauseContext *comp_elem = nullptr;
    Replaceable_elementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REDECLARE();
    antlr4::tree::TerminalNode *FINAL();
    antlr4::tree::TerminalNode *INNER();
    antlr4::tree::TerminalNode *OUTER();
    Class_definitionContext *class_definition();
    Component_clauseContext *component_clause();
    Constraining_clauseContext *constraining_clause();
    CommentContext *comment();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Replaceable_elementContext* replaceable_element();

  class  Import_clauseContext : public antlr4::ParserRuleContext {
  public:
    Import_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CommentContext *comment();
    antlr4::tree::TerminalNode *IDENT();
    Component_referenceContext *component_reference();
    Import_listContext *import_list();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Import_clauseContext* import_clause();

  class  Import_listContext : public antlr4::ParserRuleContext {
  public:
    Import_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();
    std::vector<Import_listContext *> import_list();
    Import_listContext* import_list(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Import_listContext* import_list();

  class  Extends_clauseContext : public antlr4::ParserRuleContext {
  public:
    Extends_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Component_referenceContext *component_reference();
    Class_modificationContext *class_modification();
    AnnotationContext *annotation();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Extends_clauseContext* extends_clause();

  class  Constraining_clauseContext : public antlr4::ParserRuleContext {
  public:
    Constraining_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NameContext *name();
    Class_modificationContext *class_modification();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Constraining_clauseContext* constraining_clause();

  class  Component_clauseContext : public antlr4::ParserRuleContext {
  public:
    Component_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Type_prefixContext *type_prefix();
    Type_specifierContext *type_specifier();
    Component_listContext *component_list();
    Array_subscriptsContext *array_subscripts();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_clauseContext* component_clause();

  class  Type_prefixContext : public antlr4::ParserRuleContext {
  public:
    Type_prefixContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Type_prefixContext* type_prefix();

  class  Type_specifier_elementContext : public antlr4::ParserRuleContext {
  public:
    Type_specifier_elementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Type_specifier_elementContext* type_specifier_element();

  class  Type_specifierContext : public antlr4::ParserRuleContext {
  public:
    Type_specifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Type_specifier_elementContext *> type_specifier_element();
    Type_specifier_elementContext* type_specifier_element(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Type_specifierContext* type_specifier();

  class  Component_listContext : public antlr4::ParserRuleContext {
  public:
    Component_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Component_declarationContext *> component_declaration();
    Component_declarationContext* component_declaration(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_listContext* component_list();

  class  Component_declarationContext : public antlr4::ParserRuleContext {
  public:
    Component_declarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationContext *declaration();
    CommentContext *comment();
    Condition_attributeContext *condition_attribute();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_declarationContext* component_declaration();

  class  Condition_attributeContext : public antlr4::ParserRuleContext {
  public:
    Condition_attributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Condition_attributeContext* condition_attribute();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();
    Array_subscriptsContext *array_subscripts();
    ModificationContext *modification();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationContext* declaration();

  class  ModificationContext : public antlr4::ParserRuleContext {
  public:
    ModificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ModificationContext() = default;
    void copyFrom(ModificationContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Modification_classContext : public ModificationContext {
  public:
    Modification_classContext(ModificationContext *ctx);

    Class_modificationContext *class_modification();
    ExpressionContext *expression();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Modification_assignment2Context : public ModificationContext {
  public:
    Modification_assignment2Context(ModificationContext *ctx);

    ExpressionContext *expression();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Modification_assignmentContext : public ModificationContext {
  public:
    Modification_assignmentContext(ModificationContext *ctx);

    ExpressionContext *expression();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ModificationContext* modification();

  class  Class_modificationContext : public antlr4::ParserRuleContext {
  public:
    Class_modificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Argument_listContext *argument_list();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Class_modificationContext* class_modification();

  class  Argument_listContext : public antlr4::ParserRuleContext {
  public:
    Argument_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ArgumentContext *> argument();
    ArgumentContext* argument(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Argument_listContext* argument_list();

  class  ArgumentContext : public antlr4::ParserRuleContext {
  public:
    ArgumentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Element_modification_or_replaceableContext *element_modification_or_replaceable();
    Element_redeclarationContext *element_redeclaration();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgumentContext* argument();

  class  Element_modification_or_replaceableContext : public antlr4::ParserRuleContext {
  public:
    Element_modification_or_replaceableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Element_modificationContext *element_modification();
    Element_replaceableContext *element_replaceable();
    antlr4::tree::TerminalNode *EACH();
    antlr4::tree::TerminalNode *FINAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Element_modification_or_replaceableContext* element_modification_or_replaceable();

  class  Element_modificationContext : public antlr4::ParserRuleContext {
  public:
    Element_modificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Component_referenceContext *component_reference();
    String_commentContext *string_comment();
    ModificationContext *modification();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Element_modificationContext* element_modification();

  class  Element_redeclarationContext : public antlr4::ParserRuleContext {
  public:
    Element_redeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *REDECLARE();
    Element_replaceableContext *element_replaceable();
    antlr4::tree::TerminalNode *EACH();
    antlr4::tree::TerminalNode *FINAL();
    Short_class_definitionContext *short_class_definition();
    Component_clause1Context *component_clause1();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Element_redeclarationContext* element_redeclaration();

  class  Element_replaceableContext : public antlr4::ParserRuleContext {
  public:
    Element_replaceableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Short_class_definitionContext *short_class_definition();
    Component_clause1Context *component_clause1();
    Constraining_clauseContext *constraining_clause();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Element_replaceableContext* element_replaceable();

  class  Component_clause1Context : public antlr4::ParserRuleContext {
  public:
    Component_clause1Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Type_prefixContext *type_prefix();
    Type_specifierContext *type_specifier();
    Component_declaration1Context *component_declaration1();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_clause1Context* component_clause1();

  class  Component_declaration1Context : public antlr4::ParserRuleContext {
  public:
    Component_declaration1Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationContext *declaration();
    CommentContext *comment();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_declaration1Context* component_declaration1();

  class  Short_class_definitionContext : public antlr4::ParserRuleContext {
  public:
    Short_class_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Class_prefixesContext *class_prefixes();
    antlr4::tree::TerminalNode *IDENT();
    Base_prefixContext *base_prefix();
    Component_referenceContext *component_reference();
    CommentContext *comment();
    Array_subscriptsContext *array_subscripts();
    Class_modificationContext *class_modification();
    Enum_listContext *enum_list();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Short_class_definitionContext* short_class_definition();

  class  Equation_blockContext : public antlr4::ParserRuleContext {
  public:
    Equation_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EquationContext *> equation();
    EquationContext* equation(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Equation_blockContext* equation_block();

  class  Equation_sectionContext : public antlr4::ParserRuleContext {
  public:
    Equation_sectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Equation_blockContext *equation_block();
    antlr4::tree::TerminalNode *INITIAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Equation_sectionContext* equation_section();

  class  Statement_blockContext : public antlr4::ParserRuleContext {
  public:
    Statement_blockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Statement_blockContext* statement_block();

  class  Algorithm_sectionContext : public antlr4::ParserRuleContext {
  public:
    Algorithm_sectionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Statement_blockContext *statement_block();
    antlr4::tree::TerminalNode *INITIAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Algorithm_sectionContext* algorithm_section();

  class  Equation_optionsContext : public antlr4::ParserRuleContext {
  public:
    Equation_optionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Equation_optionsContext() = default;
    void copyFrom(Equation_optionsContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Equation_whenContext : public Equation_optionsContext {
  public:
    Equation_whenContext(Equation_optionsContext *ctx);

    When_equationContext *when_equation();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equation_connect_clauseContext : public Equation_optionsContext {
  public:
    Equation_connect_clauseContext(Equation_optionsContext *ctx);

    Connect_clauseContext *connect_clause();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equation_forContext : public Equation_optionsContext {
  public:
    Equation_forContext(Equation_optionsContext *ctx);

    For_equationContext *for_equation();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equation_functionContext : public Equation_optionsContext {
  public:
    Equation_functionContext(Equation_optionsContext *ctx);

    NameContext *name();
    Function_call_argsContext *function_call_args();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equation_simpleContext : public Equation_optionsContext {
  public:
    Equation_simpleContext(Equation_optionsContext *ctx);

    Simple_expressionContext *simple_expression();
    ExpressionContext *expression();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Equation_ifContext : public Equation_optionsContext {
  public:
    Equation_ifContext(Equation_optionsContext *ctx);

    If_equationContext *if_equation();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Equation_optionsContext* equation_options();

  class  EquationContext : public antlr4::ParserRuleContext {
  public:
    EquationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Equation_optionsContext *equation_options();
    CommentContext *comment();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EquationContext* equation();

  class  Statement_optionsContext : public antlr4::ParserRuleContext {
  public:
    Statement_optionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Statement_optionsContext() = default;
    void copyFrom(Statement_optionsContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Statement_breakContext : public Statement_optionsContext {
  public:
    Statement_breakContext(Statement_optionsContext *ctx);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Statement_whileContext : public Statement_optionsContext {
  public:
    Statement_whileContext(Statement_optionsContext *ctx);

    While_statementContext *while_statement();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Statement_component_functionContext : public Statement_optionsContext {
  public:
    Statement_component_functionContext(Statement_optionsContext *ctx);

    std::vector<Component_referenceContext *> component_reference();
    Component_referenceContext* component_reference(size_t i);
    Function_call_argsContext *function_call_args();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Statement_returnContext : public Statement_optionsContext {
  public:
    Statement_returnContext(Statement_optionsContext *ctx);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Statement_forContext : public Statement_optionsContext {
  public:
    Statement_forContext(Statement_optionsContext *ctx);

    For_statementContext *for_statement();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Statement_whenContext : public Statement_optionsContext {
  public:
    Statement_whenContext(Statement_optionsContext *ctx);

    When_statementContext *when_statement();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Statement_ifContext : public Statement_optionsContext {
  public:
    Statement_ifContext(Statement_optionsContext *ctx);

    If_statementContext *if_statement();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Statement_component_referenceContext : public Statement_optionsContext {
  public:
    Statement_component_referenceContext(Statement_optionsContext *ctx);

    Component_referenceContext *component_reference();
    ExpressionContext *expression();
    Function_call_argsContext *function_call_args();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Statement_optionsContext* statement_options();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Statement_optionsContext *statement_options();
    CommentContext *comment();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  If_equationContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::ExpressionContext *expressionContext = nullptr;
    std::vector<ExpressionContext *> conditions;
    ModelicaParser::Equation_blockContext *equation_blockContext = nullptr;
    std::vector<Equation_blockContext *> blocks;
    If_equationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<Equation_blockContext *> equation_block();
    Equation_blockContext* equation_block(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  If_equationContext* if_equation();

  class  If_statementContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::ExpressionContext *expressionContext = nullptr;
    std::vector<ExpressionContext *> conditions;
    ModelicaParser::Statement_blockContext *statement_blockContext = nullptr;
    std::vector<Statement_blockContext *> blocks;
    If_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<Statement_blockContext *> statement_block();
    Statement_blockContext* statement_block(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  If_statementContext* if_statement();

  class  For_equationContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::For_indicesContext *indices = nullptr;
    ModelicaParser::Equation_blockContext *block = nullptr;
    For_equationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    For_indicesContext *for_indices();
    Equation_blockContext *equation_block();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  For_equationContext* for_equation();

  class  For_statementContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::For_indicesContext *indices = nullptr;
    ModelicaParser::Statement_blockContext *block = nullptr;
    For_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    For_indicesContext *for_indices();
    Statement_blockContext *statement_block();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  For_statementContext* for_statement();

  class  For_indicesContext : public antlr4::ParserRuleContext {
  public:
    For_indicesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<For_indexContext *> for_index();
    For_indexContext* for_index(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  For_indicesContext* for_indices();

  class  For_indexContext : public antlr4::ParserRuleContext {
  public:
    For_indexContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();
    ExpressionContext *expression();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  For_indexContext* for_index();

  class  While_statementContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::ExpressionContext *condition = nullptr;
    ModelicaParser::Statement_blockContext *block = nullptr;
    While_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();
    Statement_blockContext *statement_block();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  While_statementContext* while_statement();

  class  When_equationContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::ExpressionContext *expressionContext = nullptr;
    std::vector<ExpressionContext *> conditions;
    ModelicaParser::Equation_blockContext *equation_blockContext = nullptr;
    std::vector<Equation_blockContext *> blocks;
    When_equationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<Equation_blockContext *> equation_block();
    Equation_blockContext* equation_block(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  When_equationContext* when_equation();

  class  When_statementContext : public antlr4::ParserRuleContext {
  public:
    ModelicaParser::ExpressionContext *expressionContext = nullptr;
    std::vector<ExpressionContext *> conditions;
    ModelicaParser::Statement_blockContext *statement_blockContext = nullptr;
    std::vector<Statement_blockContext *> blocks;
    When_statementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<Statement_blockContext *> statement_block();
    Statement_blockContext* statement_block(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  When_statementContext* when_statement();

  class  Connect_clauseContext : public antlr4::ParserRuleContext {
  public:
    Connect_clauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Component_referenceContext *> component_reference();
    Component_referenceContext* component_reference(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Connect_clauseContext* connect_clause();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExpressionContext() = default;
    void copyFrom(ExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Expression_ifContext : public ExpressionContext {
  public:
    Expression_ifContext(ExpressionContext *ctx);

    ModelicaParser::ExpressionContext *expressionContext = nullptr;
    std::vector<ExpressionContext *> conditions;
    std::vector<ExpressionContext *> blocks;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expression_simpleContext : public ExpressionContext {
  public:
    Expression_simpleContext(ExpressionContext *ctx);

    Simple_expressionContext *simple_expression();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpressionContext* expression();

  class  Simple_expressionContext : public antlr4::ParserRuleContext {
  public:
    Simple_expressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Simple_expressionContext* simple_expression();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExprContext() = default;
    void copyFrom(ExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Expr_addContext : public ExprContext {
  public:
    Expr_addContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_signedContext : public ExprContext {
  public:
    Expr_signedContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_expContext : public ExprContext {
  public:
    Expr_expContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<PrimaryContext *> primary();
    PrimaryContext* primary(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_orContext : public ExprContext {
  public:
    Expr_orContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_primaryContext : public ExprContext {
  public:
    Expr_primaryContext(ExprContext *ctx);

    PrimaryContext *primary();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_andContext : public ExprContext {
  public:
    Expr_andContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_relContext : public ExprContext {
  public:
    Expr_relContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_notContext : public ExprContext {
  public:
    Expr_notContext(ExprContext *ctx);

    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Expr_mulContext : public ExprContext {
  public:
    Expr_mulContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  PrimaryContext : public antlr4::ParserRuleContext {
  public:
    PrimaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PrimaryContext() = default;
    void copyFrom(PrimaryContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Primary_stringContext : public PrimaryContext {
  public:
    Primary_stringContext(PrimaryContext *ctx);

    antlr4::tree::TerminalNode *STRING();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_endContext : public PrimaryContext {
  public:
    Primary_endContext(PrimaryContext *ctx);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_output_expression_listContext : public PrimaryContext {
  public:
    Primary_output_expression_listContext(PrimaryContext *ctx);

    Output_expression_listContext *output_expression_list();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_unsigned_numberContext : public PrimaryContext {
  public:
    Primary_unsigned_numberContext(PrimaryContext *ctx);

    antlr4::tree::TerminalNode *UNSIGNED_NUMBER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_function_argumentsContext : public PrimaryContext {
  public:
    Primary_function_argumentsContext(PrimaryContext *ctx);

    Function_argumentsContext *function_arguments();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_falseContext : public PrimaryContext {
  public:
    Primary_falseContext(PrimaryContext *ctx);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_derivativeContext : public PrimaryContext {
  public:
    Primary_derivativeContext(PrimaryContext *ctx);

    Function_call_argsContext *function_call_args();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_component_referenceContext : public PrimaryContext {
  public:
    Primary_component_referenceContext(PrimaryContext *ctx);

    Component_referenceContext *component_reference();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_expression_listContext : public PrimaryContext {
  public:
    Primary_expression_listContext(PrimaryContext *ctx);

    std::vector<Expression_listContext *> expression_list();
    Expression_listContext* expression_list(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_trueContext : public PrimaryContext {
  public:
    Primary_trueContext(PrimaryContext *ctx);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_functionContext : public PrimaryContext {
  public:
    Primary_functionContext(PrimaryContext *ctx);

    Component_referenceContext *component_reference();
    Function_call_argsContext *function_call_args();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Primary_initialContext : public PrimaryContext {
  public:
    Primary_initialContext(PrimaryContext *ctx);

    antlr4::tree::TerminalNode *INITIAL();
    Function_call_argsContext *function_call_args();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PrimaryContext* primary();

  class  NameContext : public antlr4::ParserRuleContext {
  public:
    NameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENT();
    antlr4::tree::TerminalNode* IDENT(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NameContext* name();

  class  Component_reference_elementContext : public antlr4::ParserRuleContext {
  public:
    Component_reference_elementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();
    Array_subscriptsContext *array_subscripts();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_reference_elementContext* component_reference_element();

  class  Component_referenceContext : public antlr4::ParserRuleContext {
  public:
    Component_referenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Component_reference_elementContext *> component_reference_element();
    Component_reference_elementContext* component_reference_element(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Component_referenceContext* component_reference();

  class  Function_call_argsContext : public antlr4::ParserRuleContext {
  public:
    Function_call_argsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Function_argumentsContext *function_arguments();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Function_call_argsContext* function_call_args();

  class  Function_argumentsContext : public antlr4::ParserRuleContext {
  public:
    Function_argumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Function_argumentContext *> function_argument();
    Function_argumentContext* function_argument(size_t i);
    std::vector<For_indicesContext *> for_indices();
    For_indicesContext* for_indices(size_t i);
    Named_argumentsContext *named_arguments();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Function_argumentsContext* function_arguments();

  class  Named_argumentsContext : public antlr4::ParserRuleContext {
  public:
    Named_argumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Named_argumentContext *> named_argument();
    Named_argumentContext* named_argument(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Named_argumentsContext* named_arguments();

  class  Named_argumentContext : public antlr4::ParserRuleContext {
  public:
    Named_argumentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENT();
    Function_argumentContext *function_argument();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Named_argumentContext* named_argument();

  class  Function_argumentContext : public antlr4::ParserRuleContext {
  public:
    Function_argumentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Function_argumentContext() = default;
    void copyFrom(Function_argumentContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Argument_expressionContext : public Function_argumentContext {
  public:
    Argument_expressionContext(Function_argumentContext *ctx);

    ExpressionContext *expression();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Argument_functionContext : public Function_argumentContext {
  public:
    Argument_functionContext(Function_argumentContext *ctx);

    NameContext *name();
    Named_argumentsContext *named_arguments();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Function_argumentContext* function_argument();

  class  Output_expression_listContext : public antlr4::ParserRuleContext {
  public:
    Output_expression_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Output_expression_listContext* output_expression_list();

  class  Expression_listContext : public antlr4::ParserRuleContext {
  public:
    Expression_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Expression_listContext* expression_list();

  class  Array_subscriptsContext : public antlr4::ParserRuleContext {
  public:
    Array_subscriptsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SubscriptContext *> subscript();
    SubscriptContext* subscript(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Array_subscriptsContext* array_subscripts();

  class  SubscriptContext : public antlr4::ParserRuleContext {
  public:
    SubscriptContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubscriptContext* subscript();

  class  CommentContext : public antlr4::ParserRuleContext {
  public:
    CommentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    String_commentContext *string_comment();
    AnnotationContext *annotation();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CommentContext* comment();

  class  String_commentContext : public antlr4::ParserRuleContext {
  public:
    String_commentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  String_commentContext* string_comment();

  class  AnnotationContext : public antlr4::ParserRuleContext {
  public:
    AnnotationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Class_modificationContext *class_modification();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AnnotationContext* annotation();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

