
// Generated from Modelica.g4 by ANTLR 4.9.3


#include "ModelicaVisitor.h"

#include "ModelicaParser.h"


using namespace antlrcpp;
using namespace antlr4;

ModelicaParser::ModelicaParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ModelicaParser::~ModelicaParser() {
  delete _interpreter;
}

std::string ModelicaParser::getGrammarFileName() const {
  return "Modelica.g4";
}

const std::vector<std::string>& ModelicaParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& ModelicaParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- Stored_definitionContext ------------------------------------------------------------------

ModelicaParser::Stored_definitionContext::Stored_definitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Stored_definitionContext::WITHIN() {
  return getToken(ModelicaParser::WITHIN, 0);
}

std::vector<ModelicaParser::Stored_definition_classContext *> ModelicaParser::Stored_definitionContext::stored_definition_class() {
  return getRuleContexts<ModelicaParser::Stored_definition_classContext>();
}

ModelicaParser::Stored_definition_classContext* ModelicaParser::Stored_definitionContext::stored_definition_class(size_t i) {
  return getRuleContext<ModelicaParser::Stored_definition_classContext>(i);
}

ModelicaParser::Component_referenceContext* ModelicaParser::Stored_definitionContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}


size_t ModelicaParser::Stored_definitionContext::getRuleIndex() const {
  return ModelicaParser::RuleStored_definition;
}


antlrcpp::Any ModelicaParser::Stored_definitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStored_definition(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Stored_definitionContext* ModelicaParser::stored_definition() {
  Stored_definitionContext *_localctx = _tracker.createInstance<Stored_definitionContext>(_ctx, getState());
  enterRule(_localctx, 0, ModelicaParser::RuleStored_definition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(157);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::WITHIN) {
      setState(152);
      match(ModelicaParser::WITHIN);
      setState(154);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::IDENT) {
        setState(153);
        component_reference();
      }
      setState(156);
      match(ModelicaParser::T__0);
    }
    setState(162);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__1)
      | (1ULL << ModelicaParser::T__2)
      | (1ULL << ModelicaParser::T__3)
      | (1ULL << ModelicaParser::T__4)
      | (1ULL << ModelicaParser::T__5)
      | (1ULL << ModelicaParser::T__6)
      | (1ULL << ModelicaParser::T__7)
      | (1ULL << ModelicaParser::T__8)
      | (1ULL << ModelicaParser::T__9)
      | (1ULL << ModelicaParser::T__10)
      | (1ULL << ModelicaParser::T__11)
      | (1ULL << ModelicaParser::T__12))) != 0) || ((((_la - 81) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 81)) & ((1ULL << (ModelicaParser::PARTIAL - 81))
      | (1ULL << (ModelicaParser::FINAL - 81))
      | (1ULL << (ModelicaParser::ENCAPSULATED - 81)))) != 0)) {
      setState(159);
      stored_definition_class();
      setState(164);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Stored_definition_classContext ------------------------------------------------------------------

ModelicaParser::Stored_definition_classContext::Stored_definition_classContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Class_definitionContext* ModelicaParser::Stored_definition_classContext::class_definition() {
  return getRuleContext<ModelicaParser::Class_definitionContext>(0);
}

tree::TerminalNode* ModelicaParser::Stored_definition_classContext::FINAL() {
  return getToken(ModelicaParser::FINAL, 0);
}


size_t ModelicaParser::Stored_definition_classContext::getRuleIndex() const {
  return ModelicaParser::RuleStored_definition_class;
}


antlrcpp::Any ModelicaParser::Stored_definition_classContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStored_definition_class(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Stored_definition_classContext* ModelicaParser::stored_definition_class() {
  Stored_definition_classContext *_localctx = _tracker.createInstance<Stored_definition_classContext>(_ctx, getState());
  enterRule(_localctx, 2, ModelicaParser::RuleStored_definition_class);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(166);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::FINAL) {
      setState(165);
      match(ModelicaParser::FINAL);
    }
    setState(168);
    class_definition();
    setState(169);
    match(ModelicaParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Class_definitionContext ------------------------------------------------------------------

ModelicaParser::Class_definitionContext::Class_definitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Class_prefixesContext* ModelicaParser::Class_definitionContext::class_prefixes() {
  return getRuleContext<ModelicaParser::Class_prefixesContext>(0);
}

ModelicaParser::Class_specifierContext* ModelicaParser::Class_definitionContext::class_specifier() {
  return getRuleContext<ModelicaParser::Class_specifierContext>(0);
}

tree::TerminalNode* ModelicaParser::Class_definitionContext::ENCAPSULATED() {
  return getToken(ModelicaParser::ENCAPSULATED, 0);
}


size_t ModelicaParser::Class_definitionContext::getRuleIndex() const {
  return ModelicaParser::RuleClass_definition;
}


antlrcpp::Any ModelicaParser::Class_definitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_definition(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Class_definitionContext* ModelicaParser::class_definition() {
  Class_definitionContext *_localctx = _tracker.createInstance<Class_definitionContext>(_ctx, getState());
  enterRule(_localctx, 4, ModelicaParser::RuleClass_definition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::ENCAPSULATED) {
      setState(171);
      match(ModelicaParser::ENCAPSULATED);
    }
    setState(174);
    class_prefixes();
    setState(175);
    class_specifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Class_prefixesContext ------------------------------------------------------------------

ModelicaParser::Class_prefixesContext::Class_prefixesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Class_typeContext* ModelicaParser::Class_prefixesContext::class_type() {
  return getRuleContext<ModelicaParser::Class_typeContext>(0);
}

tree::TerminalNode* ModelicaParser::Class_prefixesContext::PARTIAL() {
  return getToken(ModelicaParser::PARTIAL, 0);
}


size_t ModelicaParser::Class_prefixesContext::getRuleIndex() const {
  return ModelicaParser::RuleClass_prefixes;
}


antlrcpp::Any ModelicaParser::Class_prefixesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_prefixes(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Class_prefixesContext* ModelicaParser::class_prefixes() {
  Class_prefixesContext *_localctx = _tracker.createInstance<Class_prefixesContext>(_ctx, getState());
  enterRule(_localctx, 6, ModelicaParser::RuleClass_prefixes);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(178);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::PARTIAL) {
      setState(177);
      match(ModelicaParser::PARTIAL);
    }
    setState(180);
    class_type();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Class_typeContext ------------------------------------------------------------------

ModelicaParser::Class_typeContext::Class_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::Class_typeContext::getRuleIndex() const {
  return ModelicaParser::RuleClass_type;
}


antlrcpp::Any ModelicaParser::Class_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_type(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Class_typeContext* ModelicaParser::class_type() {
  Class_typeContext *_localctx = _tracker.createInstance<Class_typeContext>(_ctx, getState());
  enterRule(_localctx, 8, ModelicaParser::RuleClass_type);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(203);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(182);
      match(ModelicaParser::T__1);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(183);
      match(ModelicaParser::T__2);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(185);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__3) {
        setState(184);
        match(ModelicaParser::T__3);
      }
      setState(187);
      match(ModelicaParser::T__4);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(188);
      match(ModelicaParser::T__5);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(190);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__6) {
        setState(189);
        match(ModelicaParser::T__6);
      }
      setState(192);
      match(ModelicaParser::T__7);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(193);
      match(ModelicaParser::T__8);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(194);
      match(ModelicaParser::T__9);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(196);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__10

      || _la == ModelicaParser::T__11) {
        setState(195);
        _la = _input->LA(1);
        if (!(_la == ModelicaParser::T__10

        || _la == ModelicaParser::T__11)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
      }
      setState(199);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__3) {
        setState(198);
        match(ModelicaParser::T__3);
      }
      setState(201);
      match(ModelicaParser::T__12);
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(202);
      match(ModelicaParser::T__3);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Class_specifierContext ------------------------------------------------------------------

ModelicaParser::Class_specifierContext::Class_specifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::Class_specifierContext::getRuleIndex() const {
  return ModelicaParser::RuleClass_specifier;
}

void ModelicaParser::Class_specifierContext::copyFrom(Class_specifierContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Class_spec_derContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ModelicaParser::Class_spec_derContext::IDENT() {
  return getTokens(ModelicaParser::IDENT);
}

tree::TerminalNode* ModelicaParser::Class_spec_derContext::IDENT(size_t i) {
  return getToken(ModelicaParser::IDENT, i);
}

ModelicaParser::NameContext* ModelicaParser::Class_spec_derContext::name() {
  return getRuleContext<ModelicaParser::NameContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::Class_spec_derContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}

ModelicaParser::Class_spec_derContext::Class_spec_derContext(Class_specifierContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Class_spec_derContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_spec_der(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Class_spec_enumContext ------------------------------------------------------------------

tree::TerminalNode* ModelicaParser::Class_spec_enumContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::CommentContext* ModelicaParser::Class_spec_enumContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}

ModelicaParser::Enum_listContext* ModelicaParser::Class_spec_enumContext::enum_list() {
  return getRuleContext<ModelicaParser::Enum_listContext>(0);
}

ModelicaParser::Class_spec_enumContext::Class_spec_enumContext(Class_specifierContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Class_spec_enumContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_spec_enum(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Class_spec_baseContext ------------------------------------------------------------------

tree::TerminalNode* ModelicaParser::Class_spec_baseContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::Base_prefixContext* ModelicaParser::Class_spec_baseContext::base_prefix() {
  return getRuleContext<ModelicaParser::Base_prefixContext>(0);
}

ModelicaParser::Component_referenceContext* ModelicaParser::Class_spec_baseContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::Class_spec_baseContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}

ModelicaParser::Class_modificationContext* ModelicaParser::Class_spec_baseContext::class_modification() {
  return getRuleContext<ModelicaParser::Class_modificationContext>(0);
}

ModelicaParser::Class_spec_baseContext::Class_spec_baseContext(Class_specifierContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Class_spec_baseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_spec_base(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Class_spec_compContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ModelicaParser::Class_spec_compContext::IDENT() {
  return getTokens(ModelicaParser::IDENT);
}

tree::TerminalNode* ModelicaParser::Class_spec_compContext::IDENT(size_t i) {
  return getToken(ModelicaParser::IDENT, i);
}

ModelicaParser::String_commentContext* ModelicaParser::Class_spec_compContext::string_comment() {
  return getRuleContext<ModelicaParser::String_commentContext>(0);
}

ModelicaParser::CompositionContext* ModelicaParser::Class_spec_compContext::composition() {
  return getRuleContext<ModelicaParser::CompositionContext>(0);
}

ModelicaParser::Class_spec_compContext::Class_spec_compContext(Class_specifierContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Class_spec_compContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_spec_comp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Class_spec_extendsContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> ModelicaParser::Class_spec_extendsContext::IDENT() {
  return getTokens(ModelicaParser::IDENT);
}

tree::TerminalNode* ModelicaParser::Class_spec_extendsContext::IDENT(size_t i) {
  return getToken(ModelicaParser::IDENT, i);
}

ModelicaParser::String_commentContext* ModelicaParser::Class_spec_extendsContext::string_comment() {
  return getRuleContext<ModelicaParser::String_commentContext>(0);
}

ModelicaParser::CompositionContext* ModelicaParser::Class_spec_extendsContext::composition() {
  return getRuleContext<ModelicaParser::CompositionContext>(0);
}

ModelicaParser::Class_modificationContext* ModelicaParser::Class_spec_extendsContext::class_modification() {
  return getRuleContext<ModelicaParser::Class_modificationContext>(0);
}

ModelicaParser::Class_spec_extendsContext::Class_spec_extendsContext(Class_specifierContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Class_spec_extendsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_spec_extends(this);
  else
    return visitor->visitChildren(this);
}
ModelicaParser::Class_specifierContext* ModelicaParser::class_specifier() {
  Class_specifierContext *_localctx = _tracker.createInstance<Class_specifierContext>(_ctx, getState());
  enterRule(_localctx, 10, ModelicaParser::RuleClass_specifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(259);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ModelicaParser::Class_spec_compContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(205);
      match(ModelicaParser::IDENT);
      setState(206);
      string_comment();
      setState(207);
      composition();
      setState(208);
      match(ModelicaParser::T__13);
      setState(209);
      match(ModelicaParser::IDENT);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ModelicaParser::Class_spec_baseContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(211);
      match(ModelicaParser::IDENT);
      setState(212);
      match(ModelicaParser::T__14);
      setState(213);
      base_prefix();
      setState(214);
      component_reference();
      setState(216);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__16) {
        setState(215);
        class_modification();
      }
      setState(218);
      comment();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ModelicaParser::Class_spec_enumContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(220);
      match(ModelicaParser::IDENT);
      setState(221);
      match(ModelicaParser::T__14);
      setState(222);
      match(ModelicaParser::T__15);
      setState(223);
      match(ModelicaParser::T__16);
      setState(228);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ModelicaParser::T__18:
        case ModelicaParser::IDENT: {
          setState(225);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == ModelicaParser::IDENT) {
            setState(224);
            enum_list();
          }
          break;
        }

        case ModelicaParser::T__17: {
          setState(227);
          match(ModelicaParser::T__17);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(230);
      match(ModelicaParser::T__18);
      setState(231);
      comment();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<ModelicaParser::Class_spec_derContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(232);
      match(ModelicaParser::IDENT);
      setState(233);
      match(ModelicaParser::T__14);
      setState(234);
      match(ModelicaParser::T__19);
      setState(235);
      match(ModelicaParser::T__16);
      setState(236);
      name();
      setState(237);
      match(ModelicaParser::T__20);
      setState(238);
      match(ModelicaParser::IDENT);
      setState(243);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ModelicaParser::T__20) {
        setState(239);
        match(ModelicaParser::T__20);
        setState(240);
        match(ModelicaParser::IDENT);
        setState(245);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(246);
      match(ModelicaParser::T__18);
      setState(247);
      comment();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<ModelicaParser::Class_spec_extendsContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(249);
      match(ModelicaParser::T__21);
      setState(250);
      match(ModelicaParser::IDENT);
      setState(252);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__16) {
        setState(251);
        class_modification();
      }
      setState(254);
      string_comment();
      setState(255);
      composition();
      setState(256);
      match(ModelicaParser::T__13);
      setState(257);
      match(ModelicaParser::IDENT);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Base_prefixContext ------------------------------------------------------------------

ModelicaParser::Base_prefixContext::Base_prefixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Type_prefixContext* ModelicaParser::Base_prefixContext::type_prefix() {
  return getRuleContext<ModelicaParser::Type_prefixContext>(0);
}


size_t ModelicaParser::Base_prefixContext::getRuleIndex() const {
  return ModelicaParser::RuleBase_prefix;
}


antlrcpp::Any ModelicaParser::Base_prefixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitBase_prefix(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Base_prefixContext* ModelicaParser::base_prefix() {
  Base_prefixContext *_localctx = _tracker.createInstance<Base_prefixContext>(_ctx, getState());
  enterRule(_localctx, 12, ModelicaParser::RuleBase_prefix);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(261);
    type_prefix();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Enum_listContext ------------------------------------------------------------------

ModelicaParser::Enum_listContext::Enum_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Enumeration_literalContext *> ModelicaParser::Enum_listContext::enumeration_literal() {
  return getRuleContexts<ModelicaParser::Enumeration_literalContext>();
}

ModelicaParser::Enumeration_literalContext* ModelicaParser::Enum_listContext::enumeration_literal(size_t i) {
  return getRuleContext<ModelicaParser::Enumeration_literalContext>(i);
}


size_t ModelicaParser::Enum_listContext::getRuleIndex() const {
  return ModelicaParser::RuleEnum_list;
}


antlrcpp::Any ModelicaParser::Enum_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEnum_list(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Enum_listContext* ModelicaParser::enum_list() {
  Enum_listContext *_localctx = _tracker.createInstance<Enum_listContext>(_ctx, getState());
  enterRule(_localctx, 14, ModelicaParser::RuleEnum_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(263);
    enumeration_literal();
    setState(268);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__20) {
      setState(264);
      match(ModelicaParser::T__20);
      setState(265);
      enumeration_literal();
      setState(270);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Enumeration_literalContext ------------------------------------------------------------------

ModelicaParser::Enumeration_literalContext::Enumeration_literalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Enumeration_literalContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::CommentContext* ModelicaParser::Enumeration_literalContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}


size_t ModelicaParser::Enumeration_literalContext::getRuleIndex() const {
  return ModelicaParser::RuleEnumeration_literal;
}


antlrcpp::Any ModelicaParser::Enumeration_literalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEnumeration_literal(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Enumeration_literalContext* ModelicaParser::enumeration_literal() {
  Enumeration_literalContext *_localctx = _tracker.createInstance<Enumeration_literalContext>(_ctx, getState());
  enterRule(_localctx, 16, ModelicaParser::RuleEnumeration_literal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(271);
    match(ModelicaParser::IDENT);
    setState(272);
    comment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CompositionContext ------------------------------------------------------------------

ModelicaParser::CompositionContext::CompositionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Element_listContext *> ModelicaParser::CompositionContext::element_list() {
  return getRuleContexts<ModelicaParser::Element_listContext>();
}

ModelicaParser::Element_listContext* ModelicaParser::CompositionContext::element_list(size_t i) {
  return getRuleContext<ModelicaParser::Element_listContext>(i);
}

std::vector<ModelicaParser::Equation_sectionContext *> ModelicaParser::CompositionContext::equation_section() {
  return getRuleContexts<ModelicaParser::Equation_sectionContext>();
}

ModelicaParser::Equation_sectionContext* ModelicaParser::CompositionContext::equation_section(size_t i) {
  return getRuleContext<ModelicaParser::Equation_sectionContext>(i);
}

std::vector<ModelicaParser::Algorithm_sectionContext *> ModelicaParser::CompositionContext::algorithm_section() {
  return getRuleContexts<ModelicaParser::Algorithm_sectionContext>();
}

ModelicaParser::Algorithm_sectionContext* ModelicaParser::CompositionContext::algorithm_section(size_t i) {
  return getRuleContext<ModelicaParser::Algorithm_sectionContext>(i);
}

std::vector<ModelicaParser::AnnotationContext *> ModelicaParser::CompositionContext::annotation() {
  return getRuleContexts<ModelicaParser::AnnotationContext>();
}

ModelicaParser::AnnotationContext* ModelicaParser::CompositionContext::annotation(size_t i) {
  return getRuleContext<ModelicaParser::AnnotationContext>(i);
}

ModelicaParser::Language_specificationContext* ModelicaParser::CompositionContext::language_specification() {
  return getRuleContext<ModelicaParser::Language_specificationContext>(0);
}

ModelicaParser::External_function_callContext* ModelicaParser::CompositionContext::external_function_call() {
  return getRuleContext<ModelicaParser::External_function_callContext>(0);
}


size_t ModelicaParser::CompositionContext::getRuleIndex() const {
  return ModelicaParser::RuleComposition;
}


antlrcpp::Any ModelicaParser::CompositionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComposition(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::CompositionContext* ModelicaParser::composition() {
  CompositionContext *_localctx = _tracker.createInstance<CompositionContext>(_ctx, getState());
  enterRule(_localctx, 18, ModelicaParser::RuleComposition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(274);
    antlrcpp::downCast<CompositionContext *>(_localctx)->epriv = element_list();
    setState(283);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__22)
      | (1ULL << ModelicaParser::T__23)
      | (1ULL << ModelicaParser::T__41)
      | (1ULL << ModelicaParser::T__42))) != 0) || _la == ModelicaParser::INITIAL) {
      setState(281);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
      case 1: {
        setState(275);
        match(ModelicaParser::T__22);
        setState(276);
        antlrcpp::downCast<CompositionContext *>(_localctx)->epub = element_list();
        break;
      }

      case 2: {
        setState(277);
        match(ModelicaParser::T__23);
        setState(278);
        antlrcpp::downCast<CompositionContext *>(_localctx)->epro = element_list();
        break;
      }

      case 3: {
        setState(279);
        equation_section();
        break;
      }

      case 4: {
        setState(280);
        algorithm_section();
        break;
      }

      default:
        break;
      }
      setState(285);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(297);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__24) {
      setState(286);
      match(ModelicaParser::T__24);
      setState(288);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::STRING) {
        setState(287);
        language_specification();
      }
      setState(291);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::IDENT) {
        setState(290);
        external_function_call();
      }
      setState(294);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__78) {
        setState(293);
        antlrcpp::downCast<CompositionContext *>(_localctx)->ext_annotation = annotation();
      }
      setState(296);
      match(ModelicaParser::T__0);
    }
    setState(302);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__78) {
      setState(299);
      antlrcpp::downCast<CompositionContext *>(_localctx)->comp_annotation = annotation();
      setState(300);
      match(ModelicaParser::T__0);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Language_specificationContext ------------------------------------------------------------------

ModelicaParser::Language_specificationContext::Language_specificationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Language_specificationContext::STRING() {
  return getToken(ModelicaParser::STRING, 0);
}


size_t ModelicaParser::Language_specificationContext::getRuleIndex() const {
  return ModelicaParser::RuleLanguage_specification;
}


antlrcpp::Any ModelicaParser::Language_specificationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitLanguage_specification(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Language_specificationContext* ModelicaParser::language_specification() {
  Language_specificationContext *_localctx = _tracker.createInstance<Language_specificationContext>(_ctx, getState());
  enterRule(_localctx, 20, ModelicaParser::RuleLanguage_specification);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(304);
    match(ModelicaParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- External_function_callContext ------------------------------------------------------------------

ModelicaParser::External_function_callContext::External_function_callContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::External_function_callContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::Component_referenceContext* ModelicaParser::External_function_callContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::Expression_listContext* ModelicaParser::External_function_callContext::expression_list() {
  return getRuleContext<ModelicaParser::Expression_listContext>(0);
}


size_t ModelicaParser::External_function_callContext::getRuleIndex() const {
  return ModelicaParser::RuleExternal_function_call;
}


antlrcpp::Any ModelicaParser::External_function_callContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExternal_function_call(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::External_function_callContext* ModelicaParser::external_function_call() {
  External_function_callContext *_localctx = _tracker.createInstance<External_function_callContext>(_ctx, getState());
  enterRule(_localctx, 22, ModelicaParser::RuleExternal_function_call);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(309);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      setState(306);
      component_reference();
      setState(307);
      match(ModelicaParser::T__14);
      break;
    }

    default:
      break;
    }
    setState(311);
    match(ModelicaParser::IDENT);
    setState(312);
    match(ModelicaParser::T__16);
    setState(314);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__13)
      | (1ULL << ModelicaParser::T__16)
      | (1ULL << ModelicaParser::T__19)
      | (1ULL << ModelicaParser::T__39)
      | (1ULL << ModelicaParser::T__55)
      | (1ULL << ModelicaParser::T__56))) != 0) || ((((_la - 71) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 71)) & ((1ULL << (ModelicaParser::T__70 - 71))
      | (1ULL << (ModelicaParser::T__73 - 71))
      | (1ULL << (ModelicaParser::T__74 - 71))
      | (1ULL << (ModelicaParser::T__75 - 71))
      | (1ULL << (ModelicaParser::T__77 - 71))
      | (1ULL << (ModelicaParser::INITIAL - 71))
      | (1ULL << (ModelicaParser::IDENT - 71))
      | (1ULL << (ModelicaParser::STRING - 71))
      | (1ULL << (ModelicaParser::UNSIGNED_NUMBER - 71)))) != 0)) {
      setState(313);
      expression_list();
    }
    setState(316);
    match(ModelicaParser::T__18);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Element_listContext ------------------------------------------------------------------

ModelicaParser::Element_listContext::Element_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ElementContext *> ModelicaParser::Element_listContext::element() {
  return getRuleContexts<ModelicaParser::ElementContext>();
}

ModelicaParser::ElementContext* ModelicaParser::Element_listContext::element(size_t i) {
  return getRuleContext<ModelicaParser::ElementContext>(i);
}


size_t ModelicaParser::Element_listContext::getRuleIndex() const {
  return ModelicaParser::RuleElement_list;
}


antlrcpp::Any ModelicaParser::Element_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitElement_list(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Element_listContext* ModelicaParser::element_list() {
  Element_listContext *_localctx = _tracker.createInstance<Element_listContext>(_ctx, getState());
  enterRule(_localctx, 24, ModelicaParser::RuleElement_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(323);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__1)
      | (1ULL << ModelicaParser::T__2)
      | (1ULL << ModelicaParser::T__3)
      | (1ULL << ModelicaParser::T__4)
      | (1ULL << ModelicaParser::T__5)
      | (1ULL << ModelicaParser::T__6)
      | (1ULL << ModelicaParser::T__7)
      | (1ULL << ModelicaParser::T__8)
      | (1ULL << ModelicaParser::T__9)
      | (1ULL << ModelicaParser::T__10)
      | (1ULL << ModelicaParser::T__11)
      | (1ULL << ModelicaParser::T__12)
      | (1ULL << ModelicaParser::T__21)
      | (1ULL << ModelicaParser::T__25)
      | (1ULL << ModelicaParser::T__26)
      | (1ULL << ModelicaParser::T__31)
      | (1ULL << ModelicaParser::T__32)
      | (1ULL << ModelicaParser::T__33)
      | (1ULL << ModelicaParser::T__34)
      | (1ULL << ModelicaParser::T__35)
      | (1ULL << ModelicaParser::T__36)
      | (1ULL << ModelicaParser::T__37))) != 0) || ((((_la - 81) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 81)) & ((1ULL << (ModelicaParser::PARTIAL - 81))
      | (1ULL << (ModelicaParser::FINAL - 81))
      | (1ULL << (ModelicaParser::ENCAPSULATED - 81))
      | (1ULL << (ModelicaParser::REDECLARE - 81))
      | (1ULL << (ModelicaParser::INNER - 81))
      | (1ULL << (ModelicaParser::OUTER - 81))
      | (1ULL << (ModelicaParser::IDENT - 81)))) != 0)) {
      setState(318);
      element();
      setState(319);
      match(ModelicaParser::T__0);
      setState(325);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementContext ------------------------------------------------------------------

ModelicaParser::ElementContext::ElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Import_clauseContext* ModelicaParser::ElementContext::import_clause() {
  return getRuleContext<ModelicaParser::Import_clauseContext>(0);
}

ModelicaParser::Extends_clauseContext* ModelicaParser::ElementContext::extends_clause() {
  return getRuleContext<ModelicaParser::Extends_clauseContext>(0);
}

ModelicaParser::Regular_elementContext* ModelicaParser::ElementContext::regular_element() {
  return getRuleContext<ModelicaParser::Regular_elementContext>(0);
}

ModelicaParser::Replaceable_elementContext* ModelicaParser::ElementContext::replaceable_element() {
  return getRuleContext<ModelicaParser::Replaceable_elementContext>(0);
}


size_t ModelicaParser::ElementContext::getRuleIndex() const {
  return ModelicaParser::RuleElement;
}


antlrcpp::Any ModelicaParser::ElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitElement(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::ElementContext* ModelicaParser::element() {
  ElementContext *_localctx = _tracker.createInstance<ElementContext>(_ctx, getState());
  enterRule(_localctx, 26, ModelicaParser::RuleElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(330);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(326);
      import_clause();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(327);
      extends_clause();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(328);
      regular_element();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(329);
      replaceable_element();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Regular_elementContext ------------------------------------------------------------------

ModelicaParser::Regular_elementContext::Regular_elementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Regular_elementContext::REDECLARE() {
  return getToken(ModelicaParser::REDECLARE, 0);
}

tree::TerminalNode* ModelicaParser::Regular_elementContext::FINAL() {
  return getToken(ModelicaParser::FINAL, 0);
}

tree::TerminalNode* ModelicaParser::Regular_elementContext::INNER() {
  return getToken(ModelicaParser::INNER, 0);
}

tree::TerminalNode* ModelicaParser::Regular_elementContext::OUTER() {
  return getToken(ModelicaParser::OUTER, 0);
}

ModelicaParser::Class_definitionContext* ModelicaParser::Regular_elementContext::class_definition() {
  return getRuleContext<ModelicaParser::Class_definitionContext>(0);
}

ModelicaParser::Component_clauseContext* ModelicaParser::Regular_elementContext::component_clause() {
  return getRuleContext<ModelicaParser::Component_clauseContext>(0);
}


size_t ModelicaParser::Regular_elementContext::getRuleIndex() const {
  return ModelicaParser::RuleRegular_element;
}


antlrcpp::Any ModelicaParser::Regular_elementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitRegular_element(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Regular_elementContext* ModelicaParser::regular_element() {
  Regular_elementContext *_localctx = _tracker.createInstance<Regular_elementContext>(_ctx, getState());
  enterRule(_localctx, 28, ModelicaParser::RuleRegular_element);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(333);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::REDECLARE) {
      setState(332);
      match(ModelicaParser::REDECLARE);
    }
    setState(336);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::FINAL) {
      setState(335);
      match(ModelicaParser::FINAL);
    }
    setState(339);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::INNER) {
      setState(338);
      match(ModelicaParser::INNER);
    }
    setState(342);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::OUTER) {
      setState(341);
      match(ModelicaParser::OUTER);
    }
    setState(346);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__1:
      case ModelicaParser::T__2:
      case ModelicaParser::T__3:
      case ModelicaParser::T__4:
      case ModelicaParser::T__5:
      case ModelicaParser::T__6:
      case ModelicaParser::T__7:
      case ModelicaParser::T__8:
      case ModelicaParser::T__9:
      case ModelicaParser::T__10:
      case ModelicaParser::T__11:
      case ModelicaParser::T__12:
      case ModelicaParser::PARTIAL:
      case ModelicaParser::ENCAPSULATED: {
        setState(344);
        antlrcpp::downCast<Regular_elementContext *>(_localctx)->class_elem = class_definition();
        break;
      }

      case ModelicaParser::T__31:
      case ModelicaParser::T__32:
      case ModelicaParser::T__33:
      case ModelicaParser::T__34:
      case ModelicaParser::T__35:
      case ModelicaParser::T__36:
      case ModelicaParser::T__37:
      case ModelicaParser::IDENT: {
        setState(345);
        antlrcpp::downCast<Regular_elementContext *>(_localctx)->comp_elem = component_clause();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Replaceable_elementContext ------------------------------------------------------------------

ModelicaParser::Replaceable_elementContext::Replaceable_elementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Replaceable_elementContext::REDECLARE() {
  return getToken(ModelicaParser::REDECLARE, 0);
}

tree::TerminalNode* ModelicaParser::Replaceable_elementContext::FINAL() {
  return getToken(ModelicaParser::FINAL, 0);
}

tree::TerminalNode* ModelicaParser::Replaceable_elementContext::INNER() {
  return getToken(ModelicaParser::INNER, 0);
}

tree::TerminalNode* ModelicaParser::Replaceable_elementContext::OUTER() {
  return getToken(ModelicaParser::OUTER, 0);
}

ModelicaParser::Class_definitionContext* ModelicaParser::Replaceable_elementContext::class_definition() {
  return getRuleContext<ModelicaParser::Class_definitionContext>(0);
}

ModelicaParser::Component_clauseContext* ModelicaParser::Replaceable_elementContext::component_clause() {
  return getRuleContext<ModelicaParser::Component_clauseContext>(0);
}

ModelicaParser::Constraining_clauseContext* ModelicaParser::Replaceable_elementContext::constraining_clause() {
  return getRuleContext<ModelicaParser::Constraining_clauseContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::Replaceable_elementContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}


size_t ModelicaParser::Replaceable_elementContext::getRuleIndex() const {
  return ModelicaParser::RuleReplaceable_element;
}


antlrcpp::Any ModelicaParser::Replaceable_elementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitReplaceable_element(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Replaceable_elementContext* ModelicaParser::replaceable_element() {
  Replaceable_elementContext *_localctx = _tracker.createInstance<Replaceable_elementContext>(_ctx, getState());
  enterRule(_localctx, 30, ModelicaParser::RuleReplaceable_element);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(349);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::REDECLARE) {
      setState(348);
      match(ModelicaParser::REDECLARE);
    }
    setState(352);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::FINAL) {
      setState(351);
      match(ModelicaParser::FINAL);
    }
    setState(355);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::INNER) {
      setState(354);
      match(ModelicaParser::INNER);
    }
    setState(358);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::OUTER) {
      setState(357);
      match(ModelicaParser::OUTER);
    }
    setState(360);
    match(ModelicaParser::T__25);
    setState(363);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__1:
      case ModelicaParser::T__2:
      case ModelicaParser::T__3:
      case ModelicaParser::T__4:
      case ModelicaParser::T__5:
      case ModelicaParser::T__6:
      case ModelicaParser::T__7:
      case ModelicaParser::T__8:
      case ModelicaParser::T__9:
      case ModelicaParser::T__10:
      case ModelicaParser::T__11:
      case ModelicaParser::T__12:
      case ModelicaParser::PARTIAL:
      case ModelicaParser::ENCAPSULATED: {
        setState(361);
        antlrcpp::downCast<Replaceable_elementContext *>(_localctx)->class_elem = class_definition();
        break;
      }

      case ModelicaParser::T__31:
      case ModelicaParser::T__32:
      case ModelicaParser::T__33:
      case ModelicaParser::T__34:
      case ModelicaParser::T__35:
      case ModelicaParser::T__36:
      case ModelicaParser::T__37:
      case ModelicaParser::IDENT: {
        setState(362);
        antlrcpp::downCast<Replaceable_elementContext *>(_localctx)->comp_elem = component_clause();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(368);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__30) {
      setState(365);
      constraining_clause();
      setState(366);
      comment();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Import_clauseContext ------------------------------------------------------------------

ModelicaParser::Import_clauseContext::Import_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::CommentContext* ModelicaParser::Import_clauseContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}

tree::TerminalNode* ModelicaParser::Import_clauseContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::Component_referenceContext* ModelicaParser::Import_clauseContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::Import_listContext* ModelicaParser::Import_clauseContext::import_list() {
  return getRuleContext<ModelicaParser::Import_listContext>(0);
}


size_t ModelicaParser::Import_clauseContext::getRuleIndex() const {
  return ModelicaParser::RuleImport_clause;
}


antlrcpp::Any ModelicaParser::Import_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitImport_clause(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Import_clauseContext* ModelicaParser::import_clause() {
  Import_clauseContext *_localctx = _tracker.createInstance<Import_clauseContext>(_ctx, getState());
  enterRule(_localctx, 32, ModelicaParser::RuleImport_clause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(370);
    match(ModelicaParser::T__26);
    setState(382);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
    case 1: {
      setState(371);
      match(ModelicaParser::IDENT);
      setState(372);
      match(ModelicaParser::T__14);
      setState(373);
      component_reference();
      break;
    }

    case 2: {
      setState(374);
      component_reference();
      setState(380);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ModelicaParser::T__27: {
          setState(375);
          match(ModelicaParser::T__27);
          break;
        }

        case ModelicaParser::T__28: {
          setState(376);
          match(ModelicaParser::T__28);
          setState(377);
          import_list();
          setState(378);
          match(ModelicaParser::T__29);
          break;
        }

        case ModelicaParser::T__0:
        case ModelicaParser::T__78:
        case ModelicaParser::STRING: {
          break;
        }

      default:
        break;
      }
      break;
    }

    default:
      break;
    }
    setState(384);
    comment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Import_listContext ------------------------------------------------------------------

ModelicaParser::Import_listContext::Import_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Import_listContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

std::vector<ModelicaParser::Import_listContext *> ModelicaParser::Import_listContext::import_list() {
  return getRuleContexts<ModelicaParser::Import_listContext>();
}

ModelicaParser::Import_listContext* ModelicaParser::Import_listContext::import_list(size_t i) {
  return getRuleContext<ModelicaParser::Import_listContext>(i);
}


size_t ModelicaParser::Import_listContext::getRuleIndex() const {
  return ModelicaParser::RuleImport_list;
}


antlrcpp::Any ModelicaParser::Import_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitImport_list(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Import_listContext* ModelicaParser::import_list() {
  Import_listContext *_localctx = _tracker.createInstance<Import_listContext>(_ctx, getState());
  enterRule(_localctx, 34, ModelicaParser::RuleImport_list);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(386);
    match(ModelicaParser::IDENT);
    setState(391);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(387);
        match(ModelicaParser::T__20);
        setState(388);
        import_list(); 
      }
      setState(393);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Extends_clauseContext ------------------------------------------------------------------

ModelicaParser::Extends_clauseContext::Extends_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Component_referenceContext* ModelicaParser::Extends_clauseContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::Class_modificationContext* ModelicaParser::Extends_clauseContext::class_modification() {
  return getRuleContext<ModelicaParser::Class_modificationContext>(0);
}

ModelicaParser::AnnotationContext* ModelicaParser::Extends_clauseContext::annotation() {
  return getRuleContext<ModelicaParser::AnnotationContext>(0);
}


size_t ModelicaParser::Extends_clauseContext::getRuleIndex() const {
  return ModelicaParser::RuleExtends_clause;
}


antlrcpp::Any ModelicaParser::Extends_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExtends_clause(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Extends_clauseContext* ModelicaParser::extends_clause() {
  Extends_clauseContext *_localctx = _tracker.createInstance<Extends_clauseContext>(_ctx, getState());
  enterRule(_localctx, 36, ModelicaParser::RuleExtends_clause);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(394);
    match(ModelicaParser::T__21);
    setState(395);
    component_reference();
    setState(397);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__16) {
      setState(396);
      class_modification();
    }
    setState(400);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__78) {
      setState(399);
      annotation();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Constraining_clauseContext ------------------------------------------------------------------

ModelicaParser::Constraining_clauseContext::Constraining_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::NameContext* ModelicaParser::Constraining_clauseContext::name() {
  return getRuleContext<ModelicaParser::NameContext>(0);
}

ModelicaParser::Class_modificationContext* ModelicaParser::Constraining_clauseContext::class_modification() {
  return getRuleContext<ModelicaParser::Class_modificationContext>(0);
}


size_t ModelicaParser::Constraining_clauseContext::getRuleIndex() const {
  return ModelicaParser::RuleConstraining_clause;
}


antlrcpp::Any ModelicaParser::Constraining_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitConstraining_clause(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Constraining_clauseContext* ModelicaParser::constraining_clause() {
  Constraining_clauseContext *_localctx = _tracker.createInstance<Constraining_clauseContext>(_ctx, getState());
  enterRule(_localctx, 38, ModelicaParser::RuleConstraining_clause);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(402);
    match(ModelicaParser::T__30);
    setState(403);
    name();
    setState(405);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__16) {
      setState(404);
      class_modification();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Component_clauseContext ------------------------------------------------------------------

ModelicaParser::Component_clauseContext::Component_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Type_prefixContext* ModelicaParser::Component_clauseContext::type_prefix() {
  return getRuleContext<ModelicaParser::Type_prefixContext>(0);
}

ModelicaParser::Type_specifierContext* ModelicaParser::Component_clauseContext::type_specifier() {
  return getRuleContext<ModelicaParser::Type_specifierContext>(0);
}

ModelicaParser::Component_listContext* ModelicaParser::Component_clauseContext::component_list() {
  return getRuleContext<ModelicaParser::Component_listContext>(0);
}

ModelicaParser::Array_subscriptsContext* ModelicaParser::Component_clauseContext::array_subscripts() {
  return getRuleContext<ModelicaParser::Array_subscriptsContext>(0);
}


size_t ModelicaParser::Component_clauseContext::getRuleIndex() const {
  return ModelicaParser::RuleComponent_clause;
}


antlrcpp::Any ModelicaParser::Component_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComponent_clause(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Component_clauseContext* ModelicaParser::component_clause() {
  Component_clauseContext *_localctx = _tracker.createInstance<Component_clauseContext>(_ctx, getState());
  enterRule(_localctx, 40, ModelicaParser::RuleComponent_clause);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(407);
    type_prefix();
    setState(408);
    type_specifier();
    setState(410);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__75) {
      setState(409);
      array_subscripts();
    }
    setState(412);
    component_list();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Type_prefixContext ------------------------------------------------------------------

ModelicaParser::Type_prefixContext::Type_prefixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::Type_prefixContext::getRuleIndex() const {
  return ModelicaParser::RuleType_prefix;
}


antlrcpp::Any ModelicaParser::Type_prefixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitType_prefix(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Type_prefixContext* ModelicaParser::type_prefix() {
  Type_prefixContext *_localctx = _tracker.createInstance<Type_prefixContext>(_ctx, getState());
  enterRule(_localctx, 42, ModelicaParser::RuleType_prefix);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(415);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__31

    || _la == ModelicaParser::T__32) {
      setState(414);
      _la = _input->LA(1);
      if (!(_la == ModelicaParser::T__31

      || _la == ModelicaParser::T__32)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(418);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__33)
      | (1ULL << ModelicaParser::T__34)
      | (1ULL << ModelicaParser::T__35))) != 0)) {
      setState(417);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << ModelicaParser::T__33)
        | (1ULL << ModelicaParser::T__34)
        | (1ULL << ModelicaParser::T__35))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(421);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__36

    || _la == ModelicaParser::T__37) {
      setState(420);
      _la = _input->LA(1);
      if (!(_la == ModelicaParser::T__36

      || _la == ModelicaParser::T__37)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Type_specifier_elementContext ------------------------------------------------------------------

ModelicaParser::Type_specifier_elementContext::Type_specifier_elementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Type_specifier_elementContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}


size_t ModelicaParser::Type_specifier_elementContext::getRuleIndex() const {
  return ModelicaParser::RuleType_specifier_element;
}


antlrcpp::Any ModelicaParser::Type_specifier_elementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitType_specifier_element(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Type_specifier_elementContext* ModelicaParser::type_specifier_element() {
  Type_specifier_elementContext *_localctx = _tracker.createInstance<Type_specifier_elementContext>(_ctx, getState());
  enterRule(_localctx, 44, ModelicaParser::RuleType_specifier_element);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(423);
    match(ModelicaParser::IDENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Type_specifierContext ------------------------------------------------------------------

ModelicaParser::Type_specifierContext::Type_specifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Type_specifier_elementContext *> ModelicaParser::Type_specifierContext::type_specifier_element() {
  return getRuleContexts<ModelicaParser::Type_specifier_elementContext>();
}

ModelicaParser::Type_specifier_elementContext* ModelicaParser::Type_specifierContext::type_specifier_element(size_t i) {
  return getRuleContext<ModelicaParser::Type_specifier_elementContext>(i);
}


size_t ModelicaParser::Type_specifierContext::getRuleIndex() const {
  return ModelicaParser::RuleType_specifier;
}


antlrcpp::Any ModelicaParser::Type_specifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitType_specifier(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Type_specifierContext* ModelicaParser::type_specifier() {
  Type_specifierContext *_localctx = _tracker.createInstance<Type_specifierContext>(_ctx, getState());
  enterRule(_localctx, 46, ModelicaParser::RuleType_specifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(425);
    type_specifier_element();
    setState(430);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__38) {
      setState(426);
      match(ModelicaParser::T__38);
      setState(427);
      type_specifier_element();
      setState(432);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Component_listContext ------------------------------------------------------------------

ModelicaParser::Component_listContext::Component_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Component_declarationContext *> ModelicaParser::Component_listContext::component_declaration() {
  return getRuleContexts<ModelicaParser::Component_declarationContext>();
}

ModelicaParser::Component_declarationContext* ModelicaParser::Component_listContext::component_declaration(size_t i) {
  return getRuleContext<ModelicaParser::Component_declarationContext>(i);
}


size_t ModelicaParser::Component_listContext::getRuleIndex() const {
  return ModelicaParser::RuleComponent_list;
}


antlrcpp::Any ModelicaParser::Component_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComponent_list(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Component_listContext* ModelicaParser::component_list() {
  Component_listContext *_localctx = _tracker.createInstance<Component_listContext>(_ctx, getState());
  enterRule(_localctx, 48, ModelicaParser::RuleComponent_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(433);
    component_declaration();
    setState(438);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__20) {
      setState(434);
      match(ModelicaParser::T__20);
      setState(435);
      component_declaration();
      setState(440);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Component_declarationContext ------------------------------------------------------------------

ModelicaParser::Component_declarationContext::Component_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::DeclarationContext* ModelicaParser::Component_declarationContext::declaration() {
  return getRuleContext<ModelicaParser::DeclarationContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::Component_declarationContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}

ModelicaParser::Condition_attributeContext* ModelicaParser::Component_declarationContext::condition_attribute() {
  return getRuleContext<ModelicaParser::Condition_attributeContext>(0);
}


size_t ModelicaParser::Component_declarationContext::getRuleIndex() const {
  return ModelicaParser::RuleComponent_declaration;
}


antlrcpp::Any ModelicaParser::Component_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComponent_declaration(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Component_declarationContext* ModelicaParser::component_declaration() {
  Component_declarationContext *_localctx = _tracker.createInstance<Component_declarationContext>(_ctx, getState());
  enterRule(_localctx, 50, ModelicaParser::RuleComponent_declaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(441);
    declaration();
    setState(443);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__39) {
      setState(442);
      condition_attribute();
    }
    setState(445);
    comment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Condition_attributeContext ------------------------------------------------------------------

ModelicaParser::Condition_attributeContext::Condition_attributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::ExpressionContext* ModelicaParser::Condition_attributeContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}


size_t ModelicaParser::Condition_attributeContext::getRuleIndex() const {
  return ModelicaParser::RuleCondition_attribute;
}


antlrcpp::Any ModelicaParser::Condition_attributeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitCondition_attribute(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Condition_attributeContext* ModelicaParser::condition_attribute() {
  Condition_attributeContext *_localctx = _tracker.createInstance<Condition_attributeContext>(_ctx, getState());
  enterRule(_localctx, 52, ModelicaParser::RuleCondition_attribute);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(447);
    match(ModelicaParser::T__39);
    setState(448);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

ModelicaParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::DeclarationContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::Array_subscriptsContext* ModelicaParser::DeclarationContext::array_subscripts() {
  return getRuleContext<ModelicaParser::Array_subscriptsContext>(0);
}

ModelicaParser::ModificationContext* ModelicaParser::DeclarationContext::modification() {
  return getRuleContext<ModelicaParser::ModificationContext>(0);
}


size_t ModelicaParser::DeclarationContext::getRuleIndex() const {
  return ModelicaParser::RuleDeclaration;
}


antlrcpp::Any ModelicaParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::DeclarationContext* ModelicaParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 54, ModelicaParser::RuleDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(450);
    match(ModelicaParser::IDENT);
    setState(452);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__75) {
      setState(451);
      array_subscripts();
    }
    setState(455);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__14)
      | (1ULL << ModelicaParser::T__16)
      | (1ULL << ModelicaParser::T__40))) != 0)) {
      setState(454);
      modification();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ModificationContext ------------------------------------------------------------------

ModelicaParser::ModificationContext::ModificationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::ModificationContext::getRuleIndex() const {
  return ModelicaParser::RuleModification;
}

void ModelicaParser::ModificationContext::copyFrom(ModificationContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Modification_classContext ------------------------------------------------------------------

ModelicaParser::Class_modificationContext* ModelicaParser::Modification_classContext::class_modification() {
  return getRuleContext<ModelicaParser::Class_modificationContext>(0);
}

ModelicaParser::ExpressionContext* ModelicaParser::Modification_classContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}

ModelicaParser::Modification_classContext::Modification_classContext(ModificationContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Modification_classContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitModification_class(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Modification_assignment2Context ------------------------------------------------------------------

ModelicaParser::ExpressionContext* ModelicaParser::Modification_assignment2Context::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}

ModelicaParser::Modification_assignment2Context::Modification_assignment2Context(ModificationContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Modification_assignment2Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitModification_assignment2(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Modification_assignmentContext ------------------------------------------------------------------

ModelicaParser::ExpressionContext* ModelicaParser::Modification_assignmentContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}

ModelicaParser::Modification_assignmentContext::Modification_assignmentContext(ModificationContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Modification_assignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitModification_assignment(this);
  else
    return visitor->visitChildren(this);
}
ModelicaParser::ModificationContext* ModelicaParser::modification() {
  ModificationContext *_localctx = _tracker.createInstance<ModificationContext>(_ctx, getState());
  enterRule(_localctx, 56, ModelicaParser::RuleModification);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(466);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__16: {
        _localctx = _tracker.createInstance<ModelicaParser::Modification_classContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(457);
        class_modification();
        setState(460);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ModelicaParser::T__14) {
          setState(458);
          match(ModelicaParser::T__14);
          setState(459);
          expression();
        }
        break;
      }

      case ModelicaParser::T__14: {
        _localctx = _tracker.createInstance<ModelicaParser::Modification_assignmentContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(462);
        match(ModelicaParser::T__14);
        setState(463);
        expression();
        break;
      }

      case ModelicaParser::T__40: {
        _localctx = _tracker.createInstance<ModelicaParser::Modification_assignment2Context>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(464);
        match(ModelicaParser::T__40);
        setState(465);
        expression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Class_modificationContext ------------------------------------------------------------------

ModelicaParser::Class_modificationContext::Class_modificationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Argument_listContext* ModelicaParser::Class_modificationContext::argument_list() {
  return getRuleContext<ModelicaParser::Argument_listContext>(0);
}


size_t ModelicaParser::Class_modificationContext::getRuleIndex() const {
  return ModelicaParser::RuleClass_modification;
}


antlrcpp::Any ModelicaParser::Class_modificationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitClass_modification(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Class_modificationContext* ModelicaParser::class_modification() {
  Class_modificationContext *_localctx = _tracker.createInstance<Class_modificationContext>(_ctx, getState());
  enterRule(_localctx, 58, ModelicaParser::RuleClass_modification);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(468);
    match(ModelicaParser::T__16);
    setState(470);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 26) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 26)) & ((1ULL << (ModelicaParser::T__25 - 26))
      | (1ULL << (ModelicaParser::EACH - 26))
      | (1ULL << (ModelicaParser::FINAL - 26))
      | (1ULL << (ModelicaParser::REDECLARE - 26))
      | (1ULL << (ModelicaParser::IDENT - 26)))) != 0)) {
      setState(469);
      argument_list();
    }
    setState(472);
    match(ModelicaParser::T__18);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Argument_listContext ------------------------------------------------------------------

ModelicaParser::Argument_listContext::Argument_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ArgumentContext *> ModelicaParser::Argument_listContext::argument() {
  return getRuleContexts<ModelicaParser::ArgumentContext>();
}

ModelicaParser::ArgumentContext* ModelicaParser::Argument_listContext::argument(size_t i) {
  return getRuleContext<ModelicaParser::ArgumentContext>(i);
}


size_t ModelicaParser::Argument_listContext::getRuleIndex() const {
  return ModelicaParser::RuleArgument_list;
}


antlrcpp::Any ModelicaParser::Argument_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitArgument_list(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Argument_listContext* ModelicaParser::argument_list() {
  Argument_listContext *_localctx = _tracker.createInstance<Argument_listContext>(_ctx, getState());
  enterRule(_localctx, 60, ModelicaParser::RuleArgument_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(474);
    argument();
    setState(479);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__20) {
      setState(475);
      match(ModelicaParser::T__20);
      setState(476);
      argument();
      setState(481);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentContext ------------------------------------------------------------------

ModelicaParser::ArgumentContext::ArgumentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Element_modification_or_replaceableContext* ModelicaParser::ArgumentContext::element_modification_or_replaceable() {
  return getRuleContext<ModelicaParser::Element_modification_or_replaceableContext>(0);
}

ModelicaParser::Element_redeclarationContext* ModelicaParser::ArgumentContext::element_redeclaration() {
  return getRuleContext<ModelicaParser::Element_redeclarationContext>(0);
}


size_t ModelicaParser::ArgumentContext::getRuleIndex() const {
  return ModelicaParser::RuleArgument;
}


antlrcpp::Any ModelicaParser::ArgumentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitArgument(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::ArgumentContext* ModelicaParser::argument() {
  ArgumentContext *_localctx = _tracker.createInstance<ArgumentContext>(_ctx, getState());
  enterRule(_localctx, 62, ModelicaParser::RuleArgument);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(484);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__25:
      case ModelicaParser::EACH:
      case ModelicaParser::FINAL:
      case ModelicaParser::IDENT: {
        enterOuterAlt(_localctx, 1);
        setState(482);
        element_modification_or_replaceable();
        break;
      }

      case ModelicaParser::REDECLARE: {
        enterOuterAlt(_localctx, 2);
        setState(483);
        element_redeclaration();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Element_modification_or_replaceableContext ------------------------------------------------------------------

ModelicaParser::Element_modification_or_replaceableContext::Element_modification_or_replaceableContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Element_modificationContext* ModelicaParser::Element_modification_or_replaceableContext::element_modification() {
  return getRuleContext<ModelicaParser::Element_modificationContext>(0);
}

ModelicaParser::Element_replaceableContext* ModelicaParser::Element_modification_or_replaceableContext::element_replaceable() {
  return getRuleContext<ModelicaParser::Element_replaceableContext>(0);
}

tree::TerminalNode* ModelicaParser::Element_modification_or_replaceableContext::EACH() {
  return getToken(ModelicaParser::EACH, 0);
}

tree::TerminalNode* ModelicaParser::Element_modification_or_replaceableContext::FINAL() {
  return getToken(ModelicaParser::FINAL, 0);
}


size_t ModelicaParser::Element_modification_or_replaceableContext::getRuleIndex() const {
  return ModelicaParser::RuleElement_modification_or_replaceable;
}


antlrcpp::Any ModelicaParser::Element_modification_or_replaceableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitElement_modification_or_replaceable(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Element_modification_or_replaceableContext* ModelicaParser::element_modification_or_replaceable() {
  Element_modification_or_replaceableContext *_localctx = _tracker.createInstance<Element_modification_or_replaceableContext>(_ctx, getState());
  enterRule(_localctx, 64, ModelicaParser::RuleElement_modification_or_replaceable);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(487);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::EACH) {
      setState(486);
      match(ModelicaParser::EACH);
    }
    setState(490);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::FINAL) {
      setState(489);
      match(ModelicaParser::FINAL);
    }
    setState(494);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::IDENT: {
        setState(492);
        element_modification();
        break;
      }

      case ModelicaParser::T__25: {
        setState(493);
        element_replaceable();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Element_modificationContext ------------------------------------------------------------------

ModelicaParser::Element_modificationContext::Element_modificationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Component_referenceContext* ModelicaParser::Element_modificationContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::String_commentContext* ModelicaParser::Element_modificationContext::string_comment() {
  return getRuleContext<ModelicaParser::String_commentContext>(0);
}

ModelicaParser::ModificationContext* ModelicaParser::Element_modificationContext::modification() {
  return getRuleContext<ModelicaParser::ModificationContext>(0);
}


size_t ModelicaParser::Element_modificationContext::getRuleIndex() const {
  return ModelicaParser::RuleElement_modification;
}


antlrcpp::Any ModelicaParser::Element_modificationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitElement_modification(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Element_modificationContext* ModelicaParser::element_modification() {
  Element_modificationContext *_localctx = _tracker.createInstance<Element_modificationContext>(_ctx, getState());
  enterRule(_localctx, 66, ModelicaParser::RuleElement_modification);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(496);
    component_reference();
    setState(498);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__14)
      | (1ULL << ModelicaParser::T__16)
      | (1ULL << ModelicaParser::T__40))) != 0)) {
      setState(497);
      modification();
    }
    setState(500);
    string_comment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Element_redeclarationContext ------------------------------------------------------------------

ModelicaParser::Element_redeclarationContext::Element_redeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Element_redeclarationContext::REDECLARE() {
  return getToken(ModelicaParser::REDECLARE, 0);
}

ModelicaParser::Element_replaceableContext* ModelicaParser::Element_redeclarationContext::element_replaceable() {
  return getRuleContext<ModelicaParser::Element_replaceableContext>(0);
}

tree::TerminalNode* ModelicaParser::Element_redeclarationContext::EACH() {
  return getToken(ModelicaParser::EACH, 0);
}

tree::TerminalNode* ModelicaParser::Element_redeclarationContext::FINAL() {
  return getToken(ModelicaParser::FINAL, 0);
}

ModelicaParser::Short_class_definitionContext* ModelicaParser::Element_redeclarationContext::short_class_definition() {
  return getRuleContext<ModelicaParser::Short_class_definitionContext>(0);
}

ModelicaParser::Component_clause1Context* ModelicaParser::Element_redeclarationContext::component_clause1() {
  return getRuleContext<ModelicaParser::Component_clause1Context>(0);
}


size_t ModelicaParser::Element_redeclarationContext::getRuleIndex() const {
  return ModelicaParser::RuleElement_redeclaration;
}


antlrcpp::Any ModelicaParser::Element_redeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitElement_redeclaration(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Element_redeclarationContext* ModelicaParser::element_redeclaration() {
  Element_redeclarationContext *_localctx = _tracker.createInstance<Element_redeclarationContext>(_ctx, getState());
  enterRule(_localctx, 68, ModelicaParser::RuleElement_redeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(502);
    match(ModelicaParser::REDECLARE);
    setState(504);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::EACH) {
      setState(503);
      match(ModelicaParser::EACH);
    }
    setState(507);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::FINAL) {
      setState(506);
      match(ModelicaParser::FINAL);
    }
    setState(514);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__1:
      case ModelicaParser::T__2:
      case ModelicaParser::T__3:
      case ModelicaParser::T__4:
      case ModelicaParser::T__5:
      case ModelicaParser::T__6:
      case ModelicaParser::T__7:
      case ModelicaParser::T__8:
      case ModelicaParser::T__9:
      case ModelicaParser::T__10:
      case ModelicaParser::T__11:
      case ModelicaParser::T__12:
      case ModelicaParser::T__31:
      case ModelicaParser::T__32:
      case ModelicaParser::T__33:
      case ModelicaParser::T__34:
      case ModelicaParser::T__35:
      case ModelicaParser::T__36:
      case ModelicaParser::T__37:
      case ModelicaParser::PARTIAL:
      case ModelicaParser::IDENT: {
        setState(511);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ModelicaParser::T__1:
          case ModelicaParser::T__2:
          case ModelicaParser::T__3:
          case ModelicaParser::T__4:
          case ModelicaParser::T__5:
          case ModelicaParser::T__6:
          case ModelicaParser::T__7:
          case ModelicaParser::T__8:
          case ModelicaParser::T__9:
          case ModelicaParser::T__10:
          case ModelicaParser::T__11:
          case ModelicaParser::T__12:
          case ModelicaParser::PARTIAL: {
            setState(509);
            short_class_definition();
            break;
          }

          case ModelicaParser::T__31:
          case ModelicaParser::T__32:
          case ModelicaParser::T__33:
          case ModelicaParser::T__34:
          case ModelicaParser::T__35:
          case ModelicaParser::T__36:
          case ModelicaParser::T__37:
          case ModelicaParser::IDENT: {
            setState(510);
            component_clause1();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        break;
      }

      case ModelicaParser::T__25: {
        setState(513);
        element_replaceable();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Element_replaceableContext ------------------------------------------------------------------

ModelicaParser::Element_replaceableContext::Element_replaceableContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Short_class_definitionContext* ModelicaParser::Element_replaceableContext::short_class_definition() {
  return getRuleContext<ModelicaParser::Short_class_definitionContext>(0);
}

ModelicaParser::Component_clause1Context* ModelicaParser::Element_replaceableContext::component_clause1() {
  return getRuleContext<ModelicaParser::Component_clause1Context>(0);
}

ModelicaParser::Constraining_clauseContext* ModelicaParser::Element_replaceableContext::constraining_clause() {
  return getRuleContext<ModelicaParser::Constraining_clauseContext>(0);
}


size_t ModelicaParser::Element_replaceableContext::getRuleIndex() const {
  return ModelicaParser::RuleElement_replaceable;
}


antlrcpp::Any ModelicaParser::Element_replaceableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitElement_replaceable(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Element_replaceableContext* ModelicaParser::element_replaceable() {
  Element_replaceableContext *_localctx = _tracker.createInstance<Element_replaceableContext>(_ctx, getState());
  enterRule(_localctx, 70, ModelicaParser::RuleElement_replaceable);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(516);
    match(ModelicaParser::T__25);
    setState(519);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__1:
      case ModelicaParser::T__2:
      case ModelicaParser::T__3:
      case ModelicaParser::T__4:
      case ModelicaParser::T__5:
      case ModelicaParser::T__6:
      case ModelicaParser::T__7:
      case ModelicaParser::T__8:
      case ModelicaParser::T__9:
      case ModelicaParser::T__10:
      case ModelicaParser::T__11:
      case ModelicaParser::T__12:
      case ModelicaParser::PARTIAL: {
        setState(517);
        short_class_definition();
        break;
      }

      case ModelicaParser::T__31:
      case ModelicaParser::T__32:
      case ModelicaParser::T__33:
      case ModelicaParser::T__34:
      case ModelicaParser::T__35:
      case ModelicaParser::T__36:
      case ModelicaParser::T__37:
      case ModelicaParser::IDENT: {
        setState(518);
        component_clause1();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(522);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__30) {
      setState(521);
      constraining_clause();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Component_clause1Context ------------------------------------------------------------------

ModelicaParser::Component_clause1Context::Component_clause1Context(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Type_prefixContext* ModelicaParser::Component_clause1Context::type_prefix() {
  return getRuleContext<ModelicaParser::Type_prefixContext>(0);
}

ModelicaParser::Type_specifierContext* ModelicaParser::Component_clause1Context::type_specifier() {
  return getRuleContext<ModelicaParser::Type_specifierContext>(0);
}

ModelicaParser::Component_declaration1Context* ModelicaParser::Component_clause1Context::component_declaration1() {
  return getRuleContext<ModelicaParser::Component_declaration1Context>(0);
}


size_t ModelicaParser::Component_clause1Context::getRuleIndex() const {
  return ModelicaParser::RuleComponent_clause1;
}


antlrcpp::Any ModelicaParser::Component_clause1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComponent_clause1(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Component_clause1Context* ModelicaParser::component_clause1() {
  Component_clause1Context *_localctx = _tracker.createInstance<Component_clause1Context>(_ctx, getState());
  enterRule(_localctx, 72, ModelicaParser::RuleComponent_clause1);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(524);
    type_prefix();
    setState(525);
    type_specifier();
    setState(526);
    component_declaration1();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Component_declaration1Context ------------------------------------------------------------------

ModelicaParser::Component_declaration1Context::Component_declaration1Context(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::DeclarationContext* ModelicaParser::Component_declaration1Context::declaration() {
  return getRuleContext<ModelicaParser::DeclarationContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::Component_declaration1Context::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}


size_t ModelicaParser::Component_declaration1Context::getRuleIndex() const {
  return ModelicaParser::RuleComponent_declaration1;
}


antlrcpp::Any ModelicaParser::Component_declaration1Context::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComponent_declaration1(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Component_declaration1Context* ModelicaParser::component_declaration1() {
  Component_declaration1Context *_localctx = _tracker.createInstance<Component_declaration1Context>(_ctx, getState());
  enterRule(_localctx, 74, ModelicaParser::RuleComponent_declaration1);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(528);
    declaration();
    setState(529);
    comment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Short_class_definitionContext ------------------------------------------------------------------

ModelicaParser::Short_class_definitionContext::Short_class_definitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Class_prefixesContext* ModelicaParser::Short_class_definitionContext::class_prefixes() {
  return getRuleContext<ModelicaParser::Class_prefixesContext>(0);
}

tree::TerminalNode* ModelicaParser::Short_class_definitionContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::Base_prefixContext* ModelicaParser::Short_class_definitionContext::base_prefix() {
  return getRuleContext<ModelicaParser::Base_prefixContext>(0);
}

ModelicaParser::Component_referenceContext* ModelicaParser::Short_class_definitionContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::Short_class_definitionContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}

ModelicaParser::Array_subscriptsContext* ModelicaParser::Short_class_definitionContext::array_subscripts() {
  return getRuleContext<ModelicaParser::Array_subscriptsContext>(0);
}

ModelicaParser::Class_modificationContext* ModelicaParser::Short_class_definitionContext::class_modification() {
  return getRuleContext<ModelicaParser::Class_modificationContext>(0);
}

ModelicaParser::Enum_listContext* ModelicaParser::Short_class_definitionContext::enum_list() {
  return getRuleContext<ModelicaParser::Enum_listContext>(0);
}


size_t ModelicaParser::Short_class_definitionContext::getRuleIndex() const {
  return ModelicaParser::RuleShort_class_definition;
}


antlrcpp::Any ModelicaParser::Short_class_definitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitShort_class_definition(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Short_class_definitionContext* ModelicaParser::short_class_definition() {
  Short_class_definitionContext *_localctx = _tracker.createInstance<Short_class_definitionContext>(_ctx, getState());
  enterRule(_localctx, 76, ModelicaParser::RuleShort_class_definition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(531);
    class_prefixes();
    setState(532);
    match(ModelicaParser::IDENT);
    setState(533);
    match(ModelicaParser::T__14);
    setState(554);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__31:
      case ModelicaParser::T__32:
      case ModelicaParser::T__33:
      case ModelicaParser::T__34:
      case ModelicaParser::T__35:
      case ModelicaParser::T__36:
      case ModelicaParser::T__37:
      case ModelicaParser::IDENT: {
        setState(534);
        base_prefix();
        setState(535);
        component_reference();
        setState(537);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ModelicaParser::T__75) {
          setState(536);
          array_subscripts();
        }
        setState(540);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ModelicaParser::T__16) {
          setState(539);
          class_modification();
        }
        setState(542);
        comment();
        break;
      }

      case ModelicaParser::T__15: {
        setState(544);
        match(ModelicaParser::T__15);
        setState(545);
        match(ModelicaParser::T__16);
        setState(550);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ModelicaParser::T__18:
          case ModelicaParser::IDENT: {
            setState(547);
            _errHandler->sync(this);

            _la = _input->LA(1);
            if (_la == ModelicaParser::IDENT) {
              setState(546);
              enum_list();
            }
            break;
          }

          case ModelicaParser::T__17: {
            setState(549);
            match(ModelicaParser::T__17);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(552);
        match(ModelicaParser::T__18);
        setState(553);
        comment();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Equation_blockContext ------------------------------------------------------------------

ModelicaParser::Equation_blockContext::Equation_blockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::EquationContext *> ModelicaParser::Equation_blockContext::equation() {
  return getRuleContexts<ModelicaParser::EquationContext>();
}

ModelicaParser::EquationContext* ModelicaParser::Equation_blockContext::equation(size_t i) {
  return getRuleContext<ModelicaParser::EquationContext>(i);
}


size_t ModelicaParser::Equation_blockContext::getRuleIndex() const {
  return ModelicaParser::RuleEquation_block;
}


antlrcpp::Any ModelicaParser::Equation_blockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_block(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Equation_blockContext* ModelicaParser::equation_block() {
  Equation_blockContext *_localctx = _tracker.createInstance<Equation_blockContext>(_ctx, getState());
  enterRule(_localctx, 78, ModelicaParser::RuleEquation_block);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(561);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(556);
        equation();
        setState(557);
        match(ModelicaParser::T__0); 
      }
      setState(563);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Equation_sectionContext ------------------------------------------------------------------

ModelicaParser::Equation_sectionContext::Equation_sectionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Equation_blockContext* ModelicaParser::Equation_sectionContext::equation_block() {
  return getRuleContext<ModelicaParser::Equation_blockContext>(0);
}

tree::TerminalNode* ModelicaParser::Equation_sectionContext::INITIAL() {
  return getToken(ModelicaParser::INITIAL, 0);
}


size_t ModelicaParser::Equation_sectionContext::getRuleIndex() const {
  return ModelicaParser::RuleEquation_section;
}


antlrcpp::Any ModelicaParser::Equation_sectionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_section(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Equation_sectionContext* ModelicaParser::equation_section() {
  Equation_sectionContext *_localctx = _tracker.createInstance<Equation_sectionContext>(_ctx, getState());
  enterRule(_localctx, 80, ModelicaParser::RuleEquation_section);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(565);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::INITIAL) {
      setState(564);
      match(ModelicaParser::INITIAL);
    }
    setState(567);
    match(ModelicaParser::T__41);
    setState(568);
    equation_block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Statement_blockContext ------------------------------------------------------------------

ModelicaParser::Statement_blockContext::Statement_blockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::StatementContext *> ModelicaParser::Statement_blockContext::statement() {
  return getRuleContexts<ModelicaParser::StatementContext>();
}

ModelicaParser::StatementContext* ModelicaParser::Statement_blockContext::statement(size_t i) {
  return getRuleContext<ModelicaParser::StatementContext>(i);
}


size_t ModelicaParser::Statement_blockContext::getRuleIndex() const {
  return ModelicaParser::RuleStatement_block;
}


antlrcpp::Any ModelicaParser::Statement_blockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_block(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Statement_blockContext* ModelicaParser::statement_block() {
  Statement_blockContext *_localctx = _tracker.createInstance<Statement_blockContext>(_ctx, getState());
  enterRule(_localctx, 82, ModelicaParser::RuleStatement_block);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(575);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__16)
      | (1ULL << ModelicaParser::T__39)
      | (1ULL << ModelicaParser::T__43)
      | (1ULL << ModelicaParser::T__44)
      | (1ULL << ModelicaParser::T__48)
      | (1ULL << ModelicaParser::T__51)
      | (1ULL << ModelicaParser::T__52))) != 0) || _la == ModelicaParser::IDENT) {
      setState(570);
      statement();
      setState(571);
      match(ModelicaParser::T__0);
      setState(577);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Algorithm_sectionContext ------------------------------------------------------------------

ModelicaParser::Algorithm_sectionContext::Algorithm_sectionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Statement_blockContext* ModelicaParser::Algorithm_sectionContext::statement_block() {
  return getRuleContext<ModelicaParser::Statement_blockContext>(0);
}

tree::TerminalNode* ModelicaParser::Algorithm_sectionContext::INITIAL() {
  return getToken(ModelicaParser::INITIAL, 0);
}


size_t ModelicaParser::Algorithm_sectionContext::getRuleIndex() const {
  return ModelicaParser::RuleAlgorithm_section;
}


antlrcpp::Any ModelicaParser::Algorithm_sectionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitAlgorithm_section(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Algorithm_sectionContext* ModelicaParser::algorithm_section() {
  Algorithm_sectionContext *_localctx = _tracker.createInstance<Algorithm_sectionContext>(_ctx, getState());
  enterRule(_localctx, 84, ModelicaParser::RuleAlgorithm_section);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(579);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::INITIAL) {
      setState(578);
      match(ModelicaParser::INITIAL);
    }
    setState(581);
    match(ModelicaParser::T__42);
    setState(582);
    statement_block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Equation_optionsContext ------------------------------------------------------------------

ModelicaParser::Equation_optionsContext::Equation_optionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::Equation_optionsContext::getRuleIndex() const {
  return ModelicaParser::RuleEquation_options;
}

void ModelicaParser::Equation_optionsContext::copyFrom(Equation_optionsContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Equation_whenContext ------------------------------------------------------------------

ModelicaParser::When_equationContext* ModelicaParser::Equation_whenContext::when_equation() {
  return getRuleContext<ModelicaParser::When_equationContext>(0);
}

ModelicaParser::Equation_whenContext::Equation_whenContext(Equation_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Equation_whenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_when(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equation_connect_clauseContext ------------------------------------------------------------------

ModelicaParser::Connect_clauseContext* ModelicaParser::Equation_connect_clauseContext::connect_clause() {
  return getRuleContext<ModelicaParser::Connect_clauseContext>(0);
}

ModelicaParser::Equation_connect_clauseContext::Equation_connect_clauseContext(Equation_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Equation_connect_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_connect_clause(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equation_forContext ------------------------------------------------------------------

ModelicaParser::For_equationContext* ModelicaParser::Equation_forContext::for_equation() {
  return getRuleContext<ModelicaParser::For_equationContext>(0);
}

ModelicaParser::Equation_forContext::Equation_forContext(Equation_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Equation_forContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_for(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equation_functionContext ------------------------------------------------------------------

ModelicaParser::NameContext* ModelicaParser::Equation_functionContext::name() {
  return getRuleContext<ModelicaParser::NameContext>(0);
}

ModelicaParser::Function_call_argsContext* ModelicaParser::Equation_functionContext::function_call_args() {
  return getRuleContext<ModelicaParser::Function_call_argsContext>(0);
}

ModelicaParser::Equation_functionContext::Equation_functionContext(Equation_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Equation_functionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_function(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equation_simpleContext ------------------------------------------------------------------

ModelicaParser::Simple_expressionContext* ModelicaParser::Equation_simpleContext::simple_expression() {
  return getRuleContext<ModelicaParser::Simple_expressionContext>(0);
}

ModelicaParser::ExpressionContext* ModelicaParser::Equation_simpleContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}

ModelicaParser::Equation_simpleContext::Equation_simpleContext(Equation_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Equation_simpleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_simple(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equation_ifContext ------------------------------------------------------------------

ModelicaParser::If_equationContext* ModelicaParser::Equation_ifContext::if_equation() {
  return getRuleContext<ModelicaParser::If_equationContext>(0);
}

ModelicaParser::Equation_ifContext::Equation_ifContext(Equation_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Equation_ifContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation_if(this);
  else
    return visitor->visitChildren(this);
}
ModelicaParser::Equation_optionsContext* ModelicaParser::equation_options() {
  Equation_optionsContext *_localctx = _tracker.createInstance<Equation_optionsContext>(_ctx, getState());
  enterRule(_localctx, 86, ModelicaParser::RuleEquation_options);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(595);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ModelicaParser::Equation_simpleContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(584);
      simple_expression();
      setState(585);
      match(ModelicaParser::T__14);
      setState(586);
      expression();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ModelicaParser::Equation_ifContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(588);
      if_equation();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ModelicaParser::Equation_forContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(589);
      for_equation();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<ModelicaParser::Equation_connect_clauseContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(590);
      connect_clause();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<ModelicaParser::Equation_whenContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(591);
      when_equation();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<ModelicaParser::Equation_functionContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(592);
      name();
      setState(593);
      function_call_args();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EquationContext ------------------------------------------------------------------

ModelicaParser::EquationContext::EquationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Equation_optionsContext* ModelicaParser::EquationContext::equation_options() {
  return getRuleContext<ModelicaParser::Equation_optionsContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::EquationContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}


size_t ModelicaParser::EquationContext::getRuleIndex() const {
  return ModelicaParser::RuleEquation;
}


antlrcpp::Any ModelicaParser::EquationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitEquation(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::EquationContext* ModelicaParser::equation() {
  EquationContext *_localctx = _tracker.createInstance<EquationContext>(_ctx, getState());
  enterRule(_localctx, 88, ModelicaParser::RuleEquation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(597);
    equation_options();
    setState(598);
    comment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Statement_optionsContext ------------------------------------------------------------------

ModelicaParser::Statement_optionsContext::Statement_optionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::Statement_optionsContext::getRuleIndex() const {
  return ModelicaParser::RuleStatement_options;
}

void ModelicaParser::Statement_optionsContext::copyFrom(Statement_optionsContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Statement_breakContext ------------------------------------------------------------------

ModelicaParser::Statement_breakContext::Statement_breakContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_breakContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_break(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Statement_whileContext ------------------------------------------------------------------

ModelicaParser::While_statementContext* ModelicaParser::Statement_whileContext::while_statement() {
  return getRuleContext<ModelicaParser::While_statementContext>(0);
}

ModelicaParser::Statement_whileContext::Statement_whileContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_whileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_while(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Statement_component_functionContext ------------------------------------------------------------------

std::vector<ModelicaParser::Component_referenceContext *> ModelicaParser::Statement_component_functionContext::component_reference() {
  return getRuleContexts<ModelicaParser::Component_referenceContext>();
}

ModelicaParser::Component_referenceContext* ModelicaParser::Statement_component_functionContext::component_reference(size_t i) {
  return getRuleContext<ModelicaParser::Component_referenceContext>(i);
}

ModelicaParser::Function_call_argsContext* ModelicaParser::Statement_component_functionContext::function_call_args() {
  return getRuleContext<ModelicaParser::Function_call_argsContext>(0);
}

ModelicaParser::Statement_component_functionContext::Statement_component_functionContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_component_functionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_component_function(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Statement_returnContext ------------------------------------------------------------------

ModelicaParser::Statement_returnContext::Statement_returnContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_returnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_return(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Statement_forContext ------------------------------------------------------------------

ModelicaParser::For_statementContext* ModelicaParser::Statement_forContext::for_statement() {
  return getRuleContext<ModelicaParser::For_statementContext>(0);
}

ModelicaParser::Statement_forContext::Statement_forContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_forContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_for(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Statement_whenContext ------------------------------------------------------------------

ModelicaParser::When_statementContext* ModelicaParser::Statement_whenContext::when_statement() {
  return getRuleContext<ModelicaParser::When_statementContext>(0);
}

ModelicaParser::Statement_whenContext::Statement_whenContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_whenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_when(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Statement_ifContext ------------------------------------------------------------------

ModelicaParser::If_statementContext* ModelicaParser::Statement_ifContext::if_statement() {
  return getRuleContext<ModelicaParser::If_statementContext>(0);
}

ModelicaParser::Statement_ifContext::Statement_ifContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_ifContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_if(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Statement_component_referenceContext ------------------------------------------------------------------

ModelicaParser::Component_referenceContext* ModelicaParser::Statement_component_referenceContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::ExpressionContext* ModelicaParser::Statement_component_referenceContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}

ModelicaParser::Function_call_argsContext* ModelicaParser::Statement_component_referenceContext::function_call_args() {
  return getRuleContext<ModelicaParser::Function_call_argsContext>(0);
}

ModelicaParser::Statement_component_referenceContext::Statement_component_referenceContext(Statement_optionsContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Statement_component_referenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement_component_reference(this);
  else
    return visitor->visitChildren(this);
}
ModelicaParser::Statement_optionsContext* ModelicaParser::statement_options() {
  Statement_optionsContext *_localctx = _tracker.createInstance<Statement_optionsContext>(_ctx, getState());
  enterRule(_localctx, 90, ModelicaParser::RuleStatement_options);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(626);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::IDENT: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_component_referenceContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(600);
        component_reference();
        setState(604);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ModelicaParser::T__40: {
            setState(601);
            match(ModelicaParser::T__40);
            setState(602);
            expression();
            break;
          }

          case ModelicaParser::T__16: {
            setState(603);
            function_call_args();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        break;
      }

      case ModelicaParser::T__16: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_component_functionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(606);
        match(ModelicaParser::T__16);
        setState(607);
        component_reference();
        setState(612);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == ModelicaParser::T__20) {
          setState(608);
          match(ModelicaParser::T__20);
          setState(609);
          component_reference();
          setState(614);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(615);
        match(ModelicaParser::T__18);
        setState(616);
        match(ModelicaParser::T__40);
        setState(617);
        component_reference();
        setState(618);
        function_call_args();
        break;
      }

      case ModelicaParser::T__43: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_breakContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(620);
        match(ModelicaParser::T__43);
        break;
      }

      case ModelicaParser::T__44: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_returnContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(621);
        match(ModelicaParser::T__44);
        break;
      }

      case ModelicaParser::T__39: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_ifContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(622);
        if_statement();
        break;
      }

      case ModelicaParser::T__48: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_forContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(623);
        for_statement();
        break;
      }

      case ModelicaParser::T__51: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_whileContext>(_localctx);
        enterOuterAlt(_localctx, 7);
        setState(624);
        while_statement();
        break;
      }

      case ModelicaParser::T__52: {
        _localctx = _tracker.createInstance<ModelicaParser::Statement_whenContext>(_localctx);
        enterOuterAlt(_localctx, 8);
        setState(625);
        when_statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

ModelicaParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Statement_optionsContext* ModelicaParser::StatementContext::statement_options() {
  return getRuleContext<ModelicaParser::Statement_optionsContext>(0);
}

ModelicaParser::CommentContext* ModelicaParser::StatementContext::comment() {
  return getRuleContext<ModelicaParser::CommentContext>(0);
}


size_t ModelicaParser::StatementContext::getRuleIndex() const {
  return ModelicaParser::RuleStatement;
}


antlrcpp::Any ModelicaParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::StatementContext* ModelicaParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 92, ModelicaParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(628);
    statement_options();
    setState(629);
    comment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- If_equationContext ------------------------------------------------------------------

ModelicaParser::If_equationContext::If_equationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ExpressionContext *> ModelicaParser::If_equationContext::expression() {
  return getRuleContexts<ModelicaParser::ExpressionContext>();
}

ModelicaParser::ExpressionContext* ModelicaParser::If_equationContext::expression(size_t i) {
  return getRuleContext<ModelicaParser::ExpressionContext>(i);
}

std::vector<ModelicaParser::Equation_blockContext *> ModelicaParser::If_equationContext::equation_block() {
  return getRuleContexts<ModelicaParser::Equation_blockContext>();
}

ModelicaParser::Equation_blockContext* ModelicaParser::If_equationContext::equation_block(size_t i) {
  return getRuleContext<ModelicaParser::Equation_blockContext>(i);
}


size_t ModelicaParser::If_equationContext::getRuleIndex() const {
  return ModelicaParser::RuleIf_equation;
}


antlrcpp::Any ModelicaParser::If_equationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitIf_equation(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::If_equationContext* ModelicaParser::if_equation() {
  If_equationContext *_localctx = _tracker.createInstance<If_equationContext>(_ctx, getState());
  enterRule(_localctx, 94, ModelicaParser::RuleIf_equation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(631);
    match(ModelicaParser::T__39);
    setState(632);
    antlrcpp::downCast<If_equationContext *>(_localctx)->expressionContext = expression();
    antlrcpp::downCast<If_equationContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<If_equationContext *>(_localctx)->expressionContext);
    setState(633);
    match(ModelicaParser::T__45);
    setState(634);
    antlrcpp::downCast<If_equationContext *>(_localctx)->equation_blockContext = equation_block();
    antlrcpp::downCast<If_equationContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<If_equationContext *>(_localctx)->equation_blockContext);
    setState(642);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__46) {
      setState(635);
      match(ModelicaParser::T__46);
      setState(636);
      antlrcpp::downCast<If_equationContext *>(_localctx)->expressionContext = expression();
      antlrcpp::downCast<If_equationContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<If_equationContext *>(_localctx)->expressionContext);
      setState(637);
      match(ModelicaParser::T__45);
      setState(638);
      antlrcpp::downCast<If_equationContext *>(_localctx)->equation_blockContext = equation_block();
      antlrcpp::downCast<If_equationContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<If_equationContext *>(_localctx)->equation_blockContext);
      setState(644);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(647);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__47) {
      setState(645);
      match(ModelicaParser::T__47);
      setState(646);
      antlrcpp::downCast<If_equationContext *>(_localctx)->equation_blockContext = equation_block();
      antlrcpp::downCast<If_equationContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<If_equationContext *>(_localctx)->equation_blockContext);
    }
    setState(649);
    match(ModelicaParser::T__13);
    setState(650);
    match(ModelicaParser::T__39);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- If_statementContext ------------------------------------------------------------------

ModelicaParser::If_statementContext::If_statementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ExpressionContext *> ModelicaParser::If_statementContext::expression() {
  return getRuleContexts<ModelicaParser::ExpressionContext>();
}

ModelicaParser::ExpressionContext* ModelicaParser::If_statementContext::expression(size_t i) {
  return getRuleContext<ModelicaParser::ExpressionContext>(i);
}

std::vector<ModelicaParser::Statement_blockContext *> ModelicaParser::If_statementContext::statement_block() {
  return getRuleContexts<ModelicaParser::Statement_blockContext>();
}

ModelicaParser::Statement_blockContext* ModelicaParser::If_statementContext::statement_block(size_t i) {
  return getRuleContext<ModelicaParser::Statement_blockContext>(i);
}


size_t ModelicaParser::If_statementContext::getRuleIndex() const {
  return ModelicaParser::RuleIf_statement;
}


antlrcpp::Any ModelicaParser::If_statementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitIf_statement(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::If_statementContext* ModelicaParser::if_statement() {
  If_statementContext *_localctx = _tracker.createInstance<If_statementContext>(_ctx, getState());
  enterRule(_localctx, 96, ModelicaParser::RuleIf_statement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(652);
    match(ModelicaParser::T__39);
    setState(653);
    antlrcpp::downCast<If_statementContext *>(_localctx)->expressionContext = expression();
    antlrcpp::downCast<If_statementContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<If_statementContext *>(_localctx)->expressionContext);
    setState(654);
    match(ModelicaParser::T__45);
    setState(655);
    antlrcpp::downCast<If_statementContext *>(_localctx)->statement_blockContext = statement_block();
    antlrcpp::downCast<If_statementContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<If_statementContext *>(_localctx)->statement_blockContext);
    setState(663);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__46) {
      setState(656);
      match(ModelicaParser::T__46);
      setState(657);
      antlrcpp::downCast<If_statementContext *>(_localctx)->expressionContext = expression();
      antlrcpp::downCast<If_statementContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<If_statementContext *>(_localctx)->expressionContext);
      setState(658);
      match(ModelicaParser::T__45);
      setState(659);
      antlrcpp::downCast<If_statementContext *>(_localctx)->statement_blockContext = statement_block();
      antlrcpp::downCast<If_statementContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<If_statementContext *>(_localctx)->statement_blockContext);
      setState(665);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(668);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__47) {
      setState(666);
      match(ModelicaParser::T__47);
      setState(667);
      antlrcpp::downCast<If_statementContext *>(_localctx)->statement_blockContext = statement_block();
      antlrcpp::downCast<If_statementContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<If_statementContext *>(_localctx)->statement_blockContext);
    }
    setState(670);
    match(ModelicaParser::T__13);
    setState(671);
    match(ModelicaParser::T__39);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- For_equationContext ------------------------------------------------------------------

ModelicaParser::For_equationContext::For_equationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::For_indicesContext* ModelicaParser::For_equationContext::for_indices() {
  return getRuleContext<ModelicaParser::For_indicesContext>(0);
}

ModelicaParser::Equation_blockContext* ModelicaParser::For_equationContext::equation_block() {
  return getRuleContext<ModelicaParser::Equation_blockContext>(0);
}


size_t ModelicaParser::For_equationContext::getRuleIndex() const {
  return ModelicaParser::RuleFor_equation;
}


antlrcpp::Any ModelicaParser::For_equationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitFor_equation(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::For_equationContext* ModelicaParser::for_equation() {
  For_equationContext *_localctx = _tracker.createInstance<For_equationContext>(_ctx, getState());
  enterRule(_localctx, 98, ModelicaParser::RuleFor_equation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(673);
    match(ModelicaParser::T__48);
    setState(674);
    antlrcpp::downCast<For_equationContext *>(_localctx)->indices = for_indices();
    setState(675);
    match(ModelicaParser::T__49);
    setState(676);
    antlrcpp::downCast<For_equationContext *>(_localctx)->block = equation_block();
    setState(677);
    match(ModelicaParser::T__13);
    setState(678);
    match(ModelicaParser::T__48);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- For_statementContext ------------------------------------------------------------------

ModelicaParser::For_statementContext::For_statementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::For_indicesContext* ModelicaParser::For_statementContext::for_indices() {
  return getRuleContext<ModelicaParser::For_indicesContext>(0);
}

ModelicaParser::Statement_blockContext* ModelicaParser::For_statementContext::statement_block() {
  return getRuleContext<ModelicaParser::Statement_blockContext>(0);
}


size_t ModelicaParser::For_statementContext::getRuleIndex() const {
  return ModelicaParser::RuleFor_statement;
}


antlrcpp::Any ModelicaParser::For_statementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitFor_statement(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::For_statementContext* ModelicaParser::for_statement() {
  For_statementContext *_localctx = _tracker.createInstance<For_statementContext>(_ctx, getState());
  enterRule(_localctx, 100, ModelicaParser::RuleFor_statement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(680);
    match(ModelicaParser::T__48);
    setState(681);
    antlrcpp::downCast<For_statementContext *>(_localctx)->indices = for_indices();
    setState(682);
    match(ModelicaParser::T__49);
    setState(683);
    antlrcpp::downCast<For_statementContext *>(_localctx)->block = statement_block();
    setState(684);
    match(ModelicaParser::T__13);
    setState(685);
    match(ModelicaParser::T__48);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- For_indicesContext ------------------------------------------------------------------

ModelicaParser::For_indicesContext::For_indicesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::For_indexContext *> ModelicaParser::For_indicesContext::for_index() {
  return getRuleContexts<ModelicaParser::For_indexContext>();
}

ModelicaParser::For_indexContext* ModelicaParser::For_indicesContext::for_index(size_t i) {
  return getRuleContext<ModelicaParser::For_indexContext>(i);
}


size_t ModelicaParser::For_indicesContext::getRuleIndex() const {
  return ModelicaParser::RuleFor_indices;
}


antlrcpp::Any ModelicaParser::For_indicesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitFor_indices(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::For_indicesContext* ModelicaParser::for_indices() {
  For_indicesContext *_localctx = _tracker.createInstance<For_indicesContext>(_ctx, getState());
  enterRule(_localctx, 102, ModelicaParser::RuleFor_indices);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(687);
    for_index();
    setState(692);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(688);
        match(ModelicaParser::T__20);
        setState(689);
        for_index(); 
      }
      setState(694);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- For_indexContext ------------------------------------------------------------------

ModelicaParser::For_indexContext::For_indexContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::For_indexContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::ExpressionContext* ModelicaParser::For_indexContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}


size_t ModelicaParser::For_indexContext::getRuleIndex() const {
  return ModelicaParser::RuleFor_index;
}


antlrcpp::Any ModelicaParser::For_indexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitFor_index(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::For_indexContext* ModelicaParser::for_index() {
  For_indexContext *_localctx = _tracker.createInstance<For_indexContext>(_ctx, getState());
  enterRule(_localctx, 104, ModelicaParser::RuleFor_index);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(695);
    match(ModelicaParser::IDENT);
    setState(698);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__50) {
      setState(696);
      match(ModelicaParser::T__50);
      setState(697);
      expression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- While_statementContext ------------------------------------------------------------------

ModelicaParser::While_statementContext::While_statementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::ExpressionContext* ModelicaParser::While_statementContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}

ModelicaParser::Statement_blockContext* ModelicaParser::While_statementContext::statement_block() {
  return getRuleContext<ModelicaParser::Statement_blockContext>(0);
}


size_t ModelicaParser::While_statementContext::getRuleIndex() const {
  return ModelicaParser::RuleWhile_statement;
}


antlrcpp::Any ModelicaParser::While_statementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitWhile_statement(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::While_statementContext* ModelicaParser::while_statement() {
  While_statementContext *_localctx = _tracker.createInstance<While_statementContext>(_ctx, getState());
  enterRule(_localctx, 106, ModelicaParser::RuleWhile_statement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(700);
    match(ModelicaParser::T__51);
    setState(701);
    antlrcpp::downCast<While_statementContext *>(_localctx)->condition = expression();
    setState(702);
    match(ModelicaParser::T__49);
    setState(703);
    antlrcpp::downCast<While_statementContext *>(_localctx)->block = statement_block();
    setState(704);
    match(ModelicaParser::T__13);
    setState(705);
    match(ModelicaParser::T__51);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- When_equationContext ------------------------------------------------------------------

ModelicaParser::When_equationContext::When_equationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ExpressionContext *> ModelicaParser::When_equationContext::expression() {
  return getRuleContexts<ModelicaParser::ExpressionContext>();
}

ModelicaParser::ExpressionContext* ModelicaParser::When_equationContext::expression(size_t i) {
  return getRuleContext<ModelicaParser::ExpressionContext>(i);
}

std::vector<ModelicaParser::Equation_blockContext *> ModelicaParser::When_equationContext::equation_block() {
  return getRuleContexts<ModelicaParser::Equation_blockContext>();
}

ModelicaParser::Equation_blockContext* ModelicaParser::When_equationContext::equation_block(size_t i) {
  return getRuleContext<ModelicaParser::Equation_blockContext>(i);
}


size_t ModelicaParser::When_equationContext::getRuleIndex() const {
  return ModelicaParser::RuleWhen_equation;
}


antlrcpp::Any ModelicaParser::When_equationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitWhen_equation(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::When_equationContext* ModelicaParser::when_equation() {
  When_equationContext *_localctx = _tracker.createInstance<When_equationContext>(_ctx, getState());
  enterRule(_localctx, 108, ModelicaParser::RuleWhen_equation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(707);
    match(ModelicaParser::T__52);
    setState(708);
    antlrcpp::downCast<When_equationContext *>(_localctx)->expressionContext = expression();
    antlrcpp::downCast<When_equationContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<When_equationContext *>(_localctx)->expressionContext);
    setState(709);
    match(ModelicaParser::T__45);
    setState(710);
    antlrcpp::downCast<When_equationContext *>(_localctx)->equation_blockContext = equation_block();
    antlrcpp::downCast<When_equationContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<When_equationContext *>(_localctx)->equation_blockContext);
    setState(718);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__53) {
      setState(711);
      match(ModelicaParser::T__53);
      setState(712);
      antlrcpp::downCast<When_equationContext *>(_localctx)->expressionContext = expression();
      antlrcpp::downCast<When_equationContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<When_equationContext *>(_localctx)->expressionContext);
      setState(713);
      match(ModelicaParser::T__45);
      setState(714);
      antlrcpp::downCast<When_equationContext *>(_localctx)->equation_blockContext = equation_block();
      antlrcpp::downCast<When_equationContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<When_equationContext *>(_localctx)->equation_blockContext);
      setState(720);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(721);
    match(ModelicaParser::T__13);
    setState(722);
    match(ModelicaParser::T__52);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- When_statementContext ------------------------------------------------------------------

ModelicaParser::When_statementContext::When_statementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ExpressionContext *> ModelicaParser::When_statementContext::expression() {
  return getRuleContexts<ModelicaParser::ExpressionContext>();
}

ModelicaParser::ExpressionContext* ModelicaParser::When_statementContext::expression(size_t i) {
  return getRuleContext<ModelicaParser::ExpressionContext>(i);
}

std::vector<ModelicaParser::Statement_blockContext *> ModelicaParser::When_statementContext::statement_block() {
  return getRuleContexts<ModelicaParser::Statement_blockContext>();
}

ModelicaParser::Statement_blockContext* ModelicaParser::When_statementContext::statement_block(size_t i) {
  return getRuleContext<ModelicaParser::Statement_blockContext>(i);
}


size_t ModelicaParser::When_statementContext::getRuleIndex() const {
  return ModelicaParser::RuleWhen_statement;
}


antlrcpp::Any ModelicaParser::When_statementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitWhen_statement(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::When_statementContext* ModelicaParser::when_statement() {
  When_statementContext *_localctx = _tracker.createInstance<When_statementContext>(_ctx, getState());
  enterRule(_localctx, 110, ModelicaParser::RuleWhen_statement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(724);
    match(ModelicaParser::T__52);
    setState(725);
    antlrcpp::downCast<When_statementContext *>(_localctx)->expressionContext = expression();
    antlrcpp::downCast<When_statementContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<When_statementContext *>(_localctx)->expressionContext);
    setState(726);
    match(ModelicaParser::T__45);
    setState(727);
    antlrcpp::downCast<When_statementContext *>(_localctx)->statement_blockContext = statement_block();
    antlrcpp::downCast<When_statementContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<When_statementContext *>(_localctx)->statement_blockContext);
    setState(735);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__53) {
      setState(728);
      match(ModelicaParser::T__53);
      setState(729);
      antlrcpp::downCast<When_statementContext *>(_localctx)->expressionContext = expression();
      antlrcpp::downCast<When_statementContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<When_statementContext *>(_localctx)->expressionContext);
      setState(730);
      match(ModelicaParser::T__45);
      setState(731);
      antlrcpp::downCast<When_statementContext *>(_localctx)->statement_blockContext = statement_block();
      antlrcpp::downCast<When_statementContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<When_statementContext *>(_localctx)->statement_blockContext);
      setState(737);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(738);
    match(ModelicaParser::T__13);
    setState(739);
    match(ModelicaParser::T__52);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Connect_clauseContext ------------------------------------------------------------------

ModelicaParser::Connect_clauseContext::Connect_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Component_referenceContext *> ModelicaParser::Connect_clauseContext::component_reference() {
  return getRuleContexts<ModelicaParser::Component_referenceContext>();
}

ModelicaParser::Component_referenceContext* ModelicaParser::Connect_clauseContext::component_reference(size_t i) {
  return getRuleContext<ModelicaParser::Component_referenceContext>(i);
}


size_t ModelicaParser::Connect_clauseContext::getRuleIndex() const {
  return ModelicaParser::RuleConnect_clause;
}


antlrcpp::Any ModelicaParser::Connect_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitConnect_clause(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Connect_clauseContext* ModelicaParser::connect_clause() {
  Connect_clauseContext *_localctx = _tracker.createInstance<Connect_clauseContext>(_ctx, getState());
  enterRule(_localctx, 112, ModelicaParser::RuleConnect_clause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(741);
    match(ModelicaParser::T__54);
    setState(742);
    match(ModelicaParser::T__16);
    setState(743);
    component_reference();
    setState(744);
    match(ModelicaParser::T__20);
    setState(745);
    component_reference();
    setState(746);
    match(ModelicaParser::T__18);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

ModelicaParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::ExpressionContext::getRuleIndex() const {
  return ModelicaParser::RuleExpression;
}

void ModelicaParser::ExpressionContext::copyFrom(ExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Expression_ifContext ------------------------------------------------------------------

std::vector<ModelicaParser::ExpressionContext *> ModelicaParser::Expression_ifContext::expression() {
  return getRuleContexts<ModelicaParser::ExpressionContext>();
}

ModelicaParser::ExpressionContext* ModelicaParser::Expression_ifContext::expression(size_t i) {
  return getRuleContext<ModelicaParser::ExpressionContext>(i);
}

ModelicaParser::Expression_ifContext::Expression_ifContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expression_ifContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpression_if(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expression_simpleContext ------------------------------------------------------------------

ModelicaParser::Simple_expressionContext* ModelicaParser::Expression_simpleContext::simple_expression() {
  return getRuleContext<ModelicaParser::Simple_expressionContext>(0);
}

ModelicaParser::Expression_simpleContext::Expression_simpleContext(ExpressionContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expression_simpleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpression_simple(this);
  else
    return visitor->visitChildren(this);
}
ModelicaParser::ExpressionContext* ModelicaParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 114, ModelicaParser::RuleExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(766);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__13:
      case ModelicaParser::T__16:
      case ModelicaParser::T__19:
      case ModelicaParser::T__55:
      case ModelicaParser::T__56:
      case ModelicaParser::T__70:
      case ModelicaParser::T__73:
      case ModelicaParser::T__74:
      case ModelicaParser::T__75:
      case ModelicaParser::T__77:
      case ModelicaParser::INITIAL:
      case ModelicaParser::IDENT:
      case ModelicaParser::STRING:
      case ModelicaParser::UNSIGNED_NUMBER: {
        _localctx = _tracker.createInstance<ModelicaParser::Expression_simpleContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(748);
        simple_expression();
        break;
      }

      case ModelicaParser::T__39: {
        _localctx = _tracker.createInstance<ModelicaParser::Expression_ifContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(749);
        match(ModelicaParser::T__39);
        setState(750);
        antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext = expression();
        antlrcpp::downCast<Expression_ifContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext);
        setState(751);
        match(ModelicaParser::T__45);
        setState(752);
        antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext = expression();
        antlrcpp::downCast<Expression_ifContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext);
        setState(760);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == ModelicaParser::T__46) {
          setState(753);
          match(ModelicaParser::T__46);
          setState(754);
          antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext = expression();
          antlrcpp::downCast<Expression_ifContext *>(_localctx)->conditions.push_back(antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext);
          setState(755);
          match(ModelicaParser::T__45);
          setState(756);
          antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext = expression();
          antlrcpp::downCast<Expression_ifContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext);
          setState(762);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(763);
        match(ModelicaParser::T__47);
        setState(764);
        antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext = expression();
        antlrcpp::downCast<Expression_ifContext *>(_localctx)->blocks.push_back(antlrcpp::downCast<Expression_ifContext *>(_localctx)->expressionContext);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Simple_expressionContext ------------------------------------------------------------------

ModelicaParser::Simple_expressionContext::Simple_expressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ExprContext *> ModelicaParser::Simple_expressionContext::expr() {
  return getRuleContexts<ModelicaParser::ExprContext>();
}

ModelicaParser::ExprContext* ModelicaParser::Simple_expressionContext::expr(size_t i) {
  return getRuleContext<ModelicaParser::ExprContext>(i);
}


size_t ModelicaParser::Simple_expressionContext::getRuleIndex() const {
  return ModelicaParser::RuleSimple_expression;
}


antlrcpp::Any ModelicaParser::Simple_expressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitSimple_expression(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Simple_expressionContext* ModelicaParser::simple_expression() {
  Simple_expressionContext *_localctx = _tracker.createInstance<Simple_expressionContext>(_ctx, getState());
  enterRule(_localctx, 116, ModelicaParser::RuleSimple_expression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(768);
    expr(0);
    setState(775);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__17) {
      setState(769);
      match(ModelicaParser::T__17);
      setState(770);
      expr(0);
      setState(773);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ModelicaParser::T__17) {
        setState(771);
        match(ModelicaParser::T__17);
        setState(772);
        expr(0);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

ModelicaParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::ExprContext::getRuleIndex() const {
  return ModelicaParser::RuleExpr;
}

void ModelicaParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Expr_addContext ------------------------------------------------------------------

std::vector<ModelicaParser::ExprContext *> ModelicaParser::Expr_addContext::expr() {
  return getRuleContexts<ModelicaParser::ExprContext>();
}

ModelicaParser::ExprContext* ModelicaParser::Expr_addContext::expr(size_t i) {
  return getRuleContext<ModelicaParser::ExprContext>(i);
}

ModelicaParser::Expr_addContext::Expr_addContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_addContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_add(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_signedContext ------------------------------------------------------------------

ModelicaParser::ExprContext* ModelicaParser::Expr_signedContext::expr() {
  return getRuleContext<ModelicaParser::ExprContext>(0);
}

ModelicaParser::Expr_signedContext::Expr_signedContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_signedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_signed(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_expContext ------------------------------------------------------------------

std::vector<ModelicaParser::PrimaryContext *> ModelicaParser::Expr_expContext::primary() {
  return getRuleContexts<ModelicaParser::PrimaryContext>();
}

ModelicaParser::PrimaryContext* ModelicaParser::Expr_expContext::primary(size_t i) {
  return getRuleContext<ModelicaParser::PrimaryContext>(i);
}

ModelicaParser::Expr_expContext::Expr_expContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_exp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_orContext ------------------------------------------------------------------

std::vector<ModelicaParser::ExprContext *> ModelicaParser::Expr_orContext::expr() {
  return getRuleContexts<ModelicaParser::ExprContext>();
}

ModelicaParser::ExprContext* ModelicaParser::Expr_orContext::expr(size_t i) {
  return getRuleContext<ModelicaParser::ExprContext>(i);
}

ModelicaParser::Expr_orContext::Expr_orContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_orContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_or(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_primaryContext ------------------------------------------------------------------

ModelicaParser::PrimaryContext* ModelicaParser::Expr_primaryContext::primary() {
  return getRuleContext<ModelicaParser::PrimaryContext>(0);
}

ModelicaParser::Expr_primaryContext::Expr_primaryContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_primaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_primary(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_andContext ------------------------------------------------------------------

std::vector<ModelicaParser::ExprContext *> ModelicaParser::Expr_andContext::expr() {
  return getRuleContexts<ModelicaParser::ExprContext>();
}

ModelicaParser::ExprContext* ModelicaParser::Expr_andContext::expr(size_t i) {
  return getRuleContext<ModelicaParser::ExprContext>(i);
}

ModelicaParser::Expr_andContext::Expr_andContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_andContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_and(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_relContext ------------------------------------------------------------------

std::vector<ModelicaParser::ExprContext *> ModelicaParser::Expr_relContext::expr() {
  return getRuleContexts<ModelicaParser::ExprContext>();
}

ModelicaParser::ExprContext* ModelicaParser::Expr_relContext::expr(size_t i) {
  return getRuleContext<ModelicaParser::ExprContext>(i);
}

ModelicaParser::Expr_relContext::Expr_relContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_relContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_rel(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_notContext ------------------------------------------------------------------

ModelicaParser::ExprContext* ModelicaParser::Expr_notContext::expr() {
  return getRuleContext<ModelicaParser::ExprContext>(0);
}

ModelicaParser::Expr_notContext::Expr_notContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_notContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_not(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Expr_mulContext ------------------------------------------------------------------

std::vector<ModelicaParser::ExprContext *> ModelicaParser::Expr_mulContext::expr() {
  return getRuleContexts<ModelicaParser::ExprContext>();
}

ModelicaParser::ExprContext* ModelicaParser::Expr_mulContext::expr(size_t i) {
  return getRuleContext<ModelicaParser::ExprContext>(i);
}

ModelicaParser::Expr_mulContext::Expr_mulContext(ExprContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Expr_mulContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpr_mul(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::ExprContext* ModelicaParser::expr() {
   return expr(0);
}

ModelicaParser::ExprContext* ModelicaParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  ModelicaParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  ModelicaParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 118;
  enterRecursionRule(_localctx, 118, ModelicaParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(787);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 95, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<Expr_signedContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(778);
      antlrcpp::downCast<Expr_signedContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == ModelicaParser::T__55

      || _la == ModelicaParser::T__56)) {
        antlrcpp::downCast<Expr_signedContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(779);
      expr(9);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<Expr_expContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(780);
      primary();
      setState(781);
      antlrcpp::downCast<Expr_expContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == ModelicaParser::T__57

      || _la == ModelicaParser::T__58)) {
        antlrcpp::downCast<Expr_expContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(782);
      primary();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<Expr_notContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(784);
      match(ModelicaParser::T__70);
      setState(785);
      expr(4);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<Expr_primaryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(786);
      primary();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(806);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 97, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(804);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 96, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Expr_mulContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(789);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(790);
          antlrcpp::downCast<Expr_mulContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << ModelicaParser::T__27)
            | (1ULL << ModelicaParser::T__59)
            | (1ULL << ModelicaParser::T__60)
            | (1ULL << ModelicaParser::T__61))) != 0))) {
            antlrcpp::downCast<Expr_mulContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(791);
          expr(8);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Expr_addContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(792);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(793);
          antlrcpp::downCast<Expr_addContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(((((_la - 56) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 56)) & ((1ULL << (ModelicaParser::T__55 - 56))
            | (1ULL << (ModelicaParser::T__56 - 56))
            | (1ULL << (ModelicaParser::T__62 - 56))
            | (1ULL << (ModelicaParser::T__63 - 56)))) != 0))) {
            antlrcpp::downCast<Expr_addContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(794);
          expr(7);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<Expr_relContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(795);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(796);
          antlrcpp::downCast<Expr_relContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(((((_la - 65) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 65)) & ((1ULL << (ModelicaParser::T__64 - 65))
            | (1ULL << (ModelicaParser::T__65 - 65))
            | (1ULL << (ModelicaParser::T__66 - 65))
            | (1ULL << (ModelicaParser::T__67 - 65))
            | (1ULL << (ModelicaParser::T__68 - 65))
            | (1ULL << (ModelicaParser::T__69 - 65)))) != 0))) {
            antlrcpp::downCast<Expr_relContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(797);
          expr(6);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<Expr_andContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(798);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(799);
          match(ModelicaParser::T__71);
          setState(800);
          expr(4);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<Expr_orContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(801);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(802);
          match(ModelicaParser::T__72);
          setState(803);
          expr(3);
          break;
        }

        default:
          break;
        } 
      }
      setState(808);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 97, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PrimaryContext ------------------------------------------------------------------

ModelicaParser::PrimaryContext::PrimaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::PrimaryContext::getRuleIndex() const {
  return ModelicaParser::RulePrimary;
}

void ModelicaParser::PrimaryContext::copyFrom(PrimaryContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Primary_stringContext ------------------------------------------------------------------

tree::TerminalNode* ModelicaParser::Primary_stringContext::STRING() {
  return getToken(ModelicaParser::STRING, 0);
}

ModelicaParser::Primary_stringContext::Primary_stringContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_stringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_string(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_endContext ------------------------------------------------------------------

ModelicaParser::Primary_endContext::Primary_endContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_endContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_end(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_output_expression_listContext ------------------------------------------------------------------

ModelicaParser::Output_expression_listContext* ModelicaParser::Primary_output_expression_listContext::output_expression_list() {
  return getRuleContext<ModelicaParser::Output_expression_listContext>(0);
}

ModelicaParser::Primary_output_expression_listContext::Primary_output_expression_listContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_output_expression_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_output_expression_list(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_unsigned_numberContext ------------------------------------------------------------------

tree::TerminalNode* ModelicaParser::Primary_unsigned_numberContext::UNSIGNED_NUMBER() {
  return getToken(ModelicaParser::UNSIGNED_NUMBER, 0);
}

ModelicaParser::Primary_unsigned_numberContext::Primary_unsigned_numberContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_unsigned_numberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_unsigned_number(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_function_argumentsContext ------------------------------------------------------------------

ModelicaParser::Function_argumentsContext* ModelicaParser::Primary_function_argumentsContext::function_arguments() {
  return getRuleContext<ModelicaParser::Function_argumentsContext>(0);
}

ModelicaParser::Primary_function_argumentsContext::Primary_function_argumentsContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_function_argumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_function_arguments(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_falseContext ------------------------------------------------------------------

ModelicaParser::Primary_falseContext::Primary_falseContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_falseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_false(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_derivativeContext ------------------------------------------------------------------

ModelicaParser::Function_call_argsContext* ModelicaParser::Primary_derivativeContext::function_call_args() {
  return getRuleContext<ModelicaParser::Function_call_argsContext>(0);
}

ModelicaParser::Primary_derivativeContext::Primary_derivativeContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_derivativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_derivative(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_component_referenceContext ------------------------------------------------------------------

ModelicaParser::Component_referenceContext* ModelicaParser::Primary_component_referenceContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::Primary_component_referenceContext::Primary_component_referenceContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_component_referenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_component_reference(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_expression_listContext ------------------------------------------------------------------

std::vector<ModelicaParser::Expression_listContext *> ModelicaParser::Primary_expression_listContext::expression_list() {
  return getRuleContexts<ModelicaParser::Expression_listContext>();
}

ModelicaParser::Expression_listContext* ModelicaParser::Primary_expression_listContext::expression_list(size_t i) {
  return getRuleContext<ModelicaParser::Expression_listContext>(i);
}

ModelicaParser::Primary_expression_listContext::Primary_expression_listContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_expression_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_expression_list(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_trueContext ------------------------------------------------------------------

ModelicaParser::Primary_trueContext::Primary_trueContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_trueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_true(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_functionContext ------------------------------------------------------------------

ModelicaParser::Component_referenceContext* ModelicaParser::Primary_functionContext::component_reference() {
  return getRuleContext<ModelicaParser::Component_referenceContext>(0);
}

ModelicaParser::Function_call_argsContext* ModelicaParser::Primary_functionContext::function_call_args() {
  return getRuleContext<ModelicaParser::Function_call_argsContext>(0);
}

ModelicaParser::Primary_functionContext::Primary_functionContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_functionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_function(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Primary_initialContext ------------------------------------------------------------------

tree::TerminalNode* ModelicaParser::Primary_initialContext::INITIAL() {
  return getToken(ModelicaParser::INITIAL, 0);
}

ModelicaParser::Function_call_argsContext* ModelicaParser::Primary_initialContext::function_call_args() {
  return getRuleContext<ModelicaParser::Function_call_argsContext>(0);
}

ModelicaParser::Primary_initialContext::Primary_initialContext(PrimaryContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Primary_initialContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitPrimary_initial(this);
  else
    return visitor->visitChildren(this);
}
ModelicaParser::PrimaryContext* ModelicaParser::primary() {
  PrimaryContext *_localctx = _tracker.createInstance<PrimaryContext>(_ctx, getState());
  enterRule(_localctx, 120, ModelicaParser::RulePrimary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(841);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 99, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_unsigned_numberContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(809);
      match(ModelicaParser::UNSIGNED_NUMBER);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_stringContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(810);
      match(ModelicaParser::STRING);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_falseContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(811);
      match(ModelicaParser::T__73);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_trueContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(812);
      match(ModelicaParser::T__74);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_functionContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(813);
      component_reference();
      setState(814);
      function_call_args();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_derivativeContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(816);
      match(ModelicaParser::T__19);
      setState(817);
      function_call_args();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_initialContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(818);
      match(ModelicaParser::INITIAL);
      setState(819);
      function_call_args();
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_component_referenceContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(820);
      component_reference();
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_output_expression_listContext>(_localctx);
      enterOuterAlt(_localctx, 9);
      setState(821);
      match(ModelicaParser::T__16);
      setState(822);
      output_expression_list();
      setState(823);
      match(ModelicaParser::T__18);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_expression_listContext>(_localctx);
      enterOuterAlt(_localctx, 10);
      setState(825);
      match(ModelicaParser::T__75);
      setState(826);
      expression_list();
      setState(831);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ModelicaParser::T__0) {
        setState(827);
        match(ModelicaParser::T__0);
        setState(828);
        expression_list();
        setState(833);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(834);
      match(ModelicaParser::T__76);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_function_argumentsContext>(_localctx);
      enterOuterAlt(_localctx, 11);
      setState(836);
      match(ModelicaParser::T__77);
      setState(837);
      function_arguments();
      setState(838);
      match(ModelicaParser::T__29);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<ModelicaParser::Primary_endContext>(_localctx);
      enterOuterAlt(_localctx, 12);
      setState(840);
      match(ModelicaParser::T__13);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NameContext ------------------------------------------------------------------

ModelicaParser::NameContext::NameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ModelicaParser::NameContext::IDENT() {
  return getTokens(ModelicaParser::IDENT);
}

tree::TerminalNode* ModelicaParser::NameContext::IDENT(size_t i) {
  return getToken(ModelicaParser::IDENT, i);
}


size_t ModelicaParser::NameContext::getRuleIndex() const {
  return ModelicaParser::RuleName;
}


antlrcpp::Any ModelicaParser::NameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitName(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::NameContext* ModelicaParser::name() {
  NameContext *_localctx = _tracker.createInstance<NameContext>(_ctx, getState());
  enterRule(_localctx, 122, ModelicaParser::RuleName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(844);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__38) {
      setState(843);
      match(ModelicaParser::T__38);
    }
    setState(846);
    match(ModelicaParser::IDENT);
    setState(851);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__38) {
      setState(847);
      match(ModelicaParser::T__38);
      setState(848);
      match(ModelicaParser::IDENT);
      setState(853);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Component_reference_elementContext ------------------------------------------------------------------

ModelicaParser::Component_reference_elementContext::Component_reference_elementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Component_reference_elementContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::Array_subscriptsContext* ModelicaParser::Component_reference_elementContext::array_subscripts() {
  return getRuleContext<ModelicaParser::Array_subscriptsContext>(0);
}


size_t ModelicaParser::Component_reference_elementContext::getRuleIndex() const {
  return ModelicaParser::RuleComponent_reference_element;
}


antlrcpp::Any ModelicaParser::Component_reference_elementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComponent_reference_element(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Component_reference_elementContext* ModelicaParser::component_reference_element() {
  Component_reference_elementContext *_localctx = _tracker.createInstance<Component_reference_elementContext>(_ctx, getState());
  enterRule(_localctx, 124, ModelicaParser::RuleComponent_reference_element);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(854);
    match(ModelicaParser::IDENT);
    setState(856);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 102, _ctx)) {
    case 1: {
      setState(855);
      array_subscripts();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Component_referenceContext ------------------------------------------------------------------

ModelicaParser::Component_referenceContext::Component_referenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Component_reference_elementContext *> ModelicaParser::Component_referenceContext::component_reference_element() {
  return getRuleContexts<ModelicaParser::Component_reference_elementContext>();
}

ModelicaParser::Component_reference_elementContext* ModelicaParser::Component_referenceContext::component_reference_element(size_t i) {
  return getRuleContext<ModelicaParser::Component_reference_elementContext>(i);
}


size_t ModelicaParser::Component_referenceContext::getRuleIndex() const {
  return ModelicaParser::RuleComponent_reference;
}


antlrcpp::Any ModelicaParser::Component_referenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComponent_reference(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Component_referenceContext* ModelicaParser::component_reference() {
  Component_referenceContext *_localctx = _tracker.createInstance<Component_referenceContext>(_ctx, getState());
  enterRule(_localctx, 126, ModelicaParser::RuleComponent_reference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(858);
    component_reference_element();
    setState(863);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 103, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(859);
        match(ModelicaParser::T__38);
        setState(860);
        component_reference_element(); 
      }
      setState(865);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 103, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Function_call_argsContext ------------------------------------------------------------------

ModelicaParser::Function_call_argsContext::Function_call_argsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Function_argumentsContext* ModelicaParser::Function_call_argsContext::function_arguments() {
  return getRuleContext<ModelicaParser::Function_argumentsContext>(0);
}


size_t ModelicaParser::Function_call_argsContext::getRuleIndex() const {
  return ModelicaParser::RuleFunction_call_args;
}


antlrcpp::Any ModelicaParser::Function_call_argsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitFunction_call_args(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Function_call_argsContext* ModelicaParser::function_call_args() {
  Function_call_argsContext *_localctx = _tracker.createInstance<Function_call_argsContext>(_ctx, getState());
  enterRule(_localctx, 128, ModelicaParser::RuleFunction_call_args);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(866);
    match(ModelicaParser::T__16);
    setState(868);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__12)
      | (1ULL << ModelicaParser::T__13)
      | (1ULL << ModelicaParser::T__16)
      | (1ULL << ModelicaParser::T__19)
      | (1ULL << ModelicaParser::T__39)
      | (1ULL << ModelicaParser::T__55)
      | (1ULL << ModelicaParser::T__56))) != 0) || ((((_la - 71) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 71)) & ((1ULL << (ModelicaParser::T__70 - 71))
      | (1ULL << (ModelicaParser::T__73 - 71))
      | (1ULL << (ModelicaParser::T__74 - 71))
      | (1ULL << (ModelicaParser::T__75 - 71))
      | (1ULL << (ModelicaParser::T__77 - 71))
      | (1ULL << (ModelicaParser::INITIAL - 71))
      | (1ULL << (ModelicaParser::IDENT - 71))
      | (1ULL << (ModelicaParser::STRING - 71))
      | (1ULL << (ModelicaParser::UNSIGNED_NUMBER - 71)))) != 0)) {
      setState(867);
      function_arguments();
    }
    setState(870);
    match(ModelicaParser::T__18);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Function_argumentsContext ------------------------------------------------------------------

ModelicaParser::Function_argumentsContext::Function_argumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Function_argumentContext *> ModelicaParser::Function_argumentsContext::function_argument() {
  return getRuleContexts<ModelicaParser::Function_argumentContext>();
}

ModelicaParser::Function_argumentContext* ModelicaParser::Function_argumentsContext::function_argument(size_t i) {
  return getRuleContext<ModelicaParser::Function_argumentContext>(i);
}

std::vector<ModelicaParser::For_indicesContext *> ModelicaParser::Function_argumentsContext::for_indices() {
  return getRuleContexts<ModelicaParser::For_indicesContext>();
}

ModelicaParser::For_indicesContext* ModelicaParser::Function_argumentsContext::for_indices(size_t i) {
  return getRuleContext<ModelicaParser::For_indicesContext>(i);
}

ModelicaParser::Named_argumentsContext* ModelicaParser::Function_argumentsContext::named_arguments() {
  return getRuleContext<ModelicaParser::Named_argumentsContext>(0);
}


size_t ModelicaParser::Function_argumentsContext::getRuleIndex() const {
  return ModelicaParser::RuleFunction_arguments;
}


antlrcpp::Any ModelicaParser::Function_argumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitFunction_arguments(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Function_argumentsContext* ModelicaParser::function_arguments() {
  Function_argumentsContext *_localctx = _tracker.createInstance<Function_argumentsContext>(_ctx, getState());
  enterRule(_localctx, 130, ModelicaParser::RuleFunction_arguments);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(883);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 107, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(872);
      function_argument();
      setState(879);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ModelicaParser::T__20

      || _la == ModelicaParser::T__48) {
        setState(877);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ModelicaParser::T__20: {
            setState(873);
            match(ModelicaParser::T__20);
            setState(874);
            function_argument();
            break;
          }

          case ModelicaParser::T__48: {
            setState(875);
            match(ModelicaParser::T__48);
            setState(876);
            for_indices();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(881);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(882);
      named_arguments();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Named_argumentsContext ------------------------------------------------------------------

ModelicaParser::Named_argumentsContext::Named_argumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::Named_argumentContext *> ModelicaParser::Named_argumentsContext::named_argument() {
  return getRuleContexts<ModelicaParser::Named_argumentContext>();
}

ModelicaParser::Named_argumentContext* ModelicaParser::Named_argumentsContext::named_argument(size_t i) {
  return getRuleContext<ModelicaParser::Named_argumentContext>(i);
}


size_t ModelicaParser::Named_argumentsContext::getRuleIndex() const {
  return ModelicaParser::RuleNamed_arguments;
}


antlrcpp::Any ModelicaParser::Named_argumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitNamed_arguments(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Named_argumentsContext* ModelicaParser::named_arguments() {
  Named_argumentsContext *_localctx = _tracker.createInstance<Named_argumentsContext>(_ctx, getState());
  enterRule(_localctx, 132, ModelicaParser::RuleNamed_arguments);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(885);
    named_argument();
    setState(890);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__20) {
      setState(886);
      match(ModelicaParser::T__20);
      setState(887);
      named_argument();
      setState(892);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Named_argumentContext ------------------------------------------------------------------

ModelicaParser::Named_argumentContext::Named_argumentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ModelicaParser::Named_argumentContext::IDENT() {
  return getToken(ModelicaParser::IDENT, 0);
}

ModelicaParser::Function_argumentContext* ModelicaParser::Named_argumentContext::function_argument() {
  return getRuleContext<ModelicaParser::Function_argumentContext>(0);
}


size_t ModelicaParser::Named_argumentContext::getRuleIndex() const {
  return ModelicaParser::RuleNamed_argument;
}


antlrcpp::Any ModelicaParser::Named_argumentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitNamed_argument(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Named_argumentContext* ModelicaParser::named_argument() {
  Named_argumentContext *_localctx = _tracker.createInstance<Named_argumentContext>(_ctx, getState());
  enterRule(_localctx, 134, ModelicaParser::RuleNamed_argument);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(893);
    match(ModelicaParser::IDENT);
    setState(894);
    match(ModelicaParser::T__14);
    setState(895);
    function_argument();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Function_argumentContext ------------------------------------------------------------------

ModelicaParser::Function_argumentContext::Function_argumentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ModelicaParser::Function_argumentContext::getRuleIndex() const {
  return ModelicaParser::RuleFunction_argument;
}

void ModelicaParser::Function_argumentContext::copyFrom(Function_argumentContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Argument_expressionContext ------------------------------------------------------------------

ModelicaParser::ExpressionContext* ModelicaParser::Argument_expressionContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}

ModelicaParser::Argument_expressionContext::Argument_expressionContext(Function_argumentContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Argument_expressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitArgument_expression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Argument_functionContext ------------------------------------------------------------------

ModelicaParser::NameContext* ModelicaParser::Argument_functionContext::name() {
  return getRuleContext<ModelicaParser::NameContext>(0);
}

ModelicaParser::Named_argumentsContext* ModelicaParser::Argument_functionContext::named_arguments() {
  return getRuleContext<ModelicaParser::Named_argumentsContext>(0);
}

ModelicaParser::Argument_functionContext::Argument_functionContext(Function_argumentContext *ctx) { copyFrom(ctx); }


antlrcpp::Any ModelicaParser::Argument_functionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitArgument_function(this);
  else
    return visitor->visitChildren(this);
}
ModelicaParser::Function_argumentContext* ModelicaParser::function_argument() {
  Function_argumentContext *_localctx = _tracker.createInstance<Function_argumentContext>(_ctx, getState());
  enterRule(_localctx, 136, ModelicaParser::RuleFunction_argument);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(906);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__12: {
        _localctx = _tracker.createInstance<ModelicaParser::Argument_functionContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(897);
        match(ModelicaParser::T__12);
        setState(898);
        name();
        setState(899);
        match(ModelicaParser::T__16);
        setState(901);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ModelicaParser::IDENT) {
          setState(900);
          named_arguments();
        }
        setState(903);
        match(ModelicaParser::T__18);
        break;
      }

      case ModelicaParser::T__13:
      case ModelicaParser::T__16:
      case ModelicaParser::T__19:
      case ModelicaParser::T__39:
      case ModelicaParser::T__55:
      case ModelicaParser::T__56:
      case ModelicaParser::T__70:
      case ModelicaParser::T__73:
      case ModelicaParser::T__74:
      case ModelicaParser::T__75:
      case ModelicaParser::T__77:
      case ModelicaParser::INITIAL:
      case ModelicaParser::IDENT:
      case ModelicaParser::STRING:
      case ModelicaParser::UNSIGNED_NUMBER: {
        _localctx = _tracker.createInstance<ModelicaParser::Argument_expressionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(905);
        expression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Output_expression_listContext ------------------------------------------------------------------

ModelicaParser::Output_expression_listContext::Output_expression_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ExpressionContext *> ModelicaParser::Output_expression_listContext::expression() {
  return getRuleContexts<ModelicaParser::ExpressionContext>();
}

ModelicaParser::ExpressionContext* ModelicaParser::Output_expression_listContext::expression(size_t i) {
  return getRuleContext<ModelicaParser::ExpressionContext>(i);
}


size_t ModelicaParser::Output_expression_listContext::getRuleIndex() const {
  return ModelicaParser::RuleOutput_expression_list;
}


antlrcpp::Any ModelicaParser::Output_expression_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitOutput_expression_list(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Output_expression_listContext* ModelicaParser::output_expression_list() {
  Output_expression_listContext *_localctx = _tracker.createInstance<Output_expression_listContext>(_ctx, getState());
  enterRule(_localctx, 138, ModelicaParser::RuleOutput_expression_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(909);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ModelicaParser::T__13)
      | (1ULL << ModelicaParser::T__16)
      | (1ULL << ModelicaParser::T__19)
      | (1ULL << ModelicaParser::T__39)
      | (1ULL << ModelicaParser::T__55)
      | (1ULL << ModelicaParser::T__56))) != 0) || ((((_la - 71) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 71)) & ((1ULL << (ModelicaParser::T__70 - 71))
      | (1ULL << (ModelicaParser::T__73 - 71))
      | (1ULL << (ModelicaParser::T__74 - 71))
      | (1ULL << (ModelicaParser::T__75 - 71))
      | (1ULL << (ModelicaParser::T__77 - 71))
      | (1ULL << (ModelicaParser::INITIAL - 71))
      | (1ULL << (ModelicaParser::IDENT - 71))
      | (1ULL << (ModelicaParser::STRING - 71))
      | (1ULL << (ModelicaParser::UNSIGNED_NUMBER - 71)))) != 0)) {
      setState(908);
      expression();
    }
    setState(915);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__20) {
      setState(911);
      match(ModelicaParser::T__20);
      setState(912);
      expression();
      setState(917);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Expression_listContext ------------------------------------------------------------------

ModelicaParser::Expression_listContext::Expression_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::ExpressionContext *> ModelicaParser::Expression_listContext::expression() {
  return getRuleContexts<ModelicaParser::ExpressionContext>();
}

ModelicaParser::ExpressionContext* ModelicaParser::Expression_listContext::expression(size_t i) {
  return getRuleContext<ModelicaParser::ExpressionContext>(i);
}


size_t ModelicaParser::Expression_listContext::getRuleIndex() const {
  return ModelicaParser::RuleExpression_list;
}


antlrcpp::Any ModelicaParser::Expression_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitExpression_list(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Expression_listContext* ModelicaParser::expression_list() {
  Expression_listContext *_localctx = _tracker.createInstance<Expression_listContext>(_ctx, getState());
  enterRule(_localctx, 140, ModelicaParser::RuleExpression_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(918);
    expression();
    setState(923);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__20) {
      setState(919);
      match(ModelicaParser::T__20);
      setState(920);
      expression();
      setState(925);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Array_subscriptsContext ------------------------------------------------------------------

ModelicaParser::Array_subscriptsContext::Array_subscriptsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ModelicaParser::SubscriptContext *> ModelicaParser::Array_subscriptsContext::subscript() {
  return getRuleContexts<ModelicaParser::SubscriptContext>();
}

ModelicaParser::SubscriptContext* ModelicaParser::Array_subscriptsContext::subscript(size_t i) {
  return getRuleContext<ModelicaParser::SubscriptContext>(i);
}


size_t ModelicaParser::Array_subscriptsContext::getRuleIndex() const {
  return ModelicaParser::RuleArray_subscripts;
}


antlrcpp::Any ModelicaParser::Array_subscriptsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitArray_subscripts(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::Array_subscriptsContext* ModelicaParser::array_subscripts() {
  Array_subscriptsContext *_localctx = _tracker.createInstance<Array_subscriptsContext>(_ctx, getState());
  enterRule(_localctx, 142, ModelicaParser::RuleArray_subscripts);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(926);
    match(ModelicaParser::T__75);
    setState(927);
    subscript();
    setState(932);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ModelicaParser::T__20) {
      setState(928);
      match(ModelicaParser::T__20);
      setState(929);
      subscript();
      setState(934);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(935);
    match(ModelicaParser::T__76);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SubscriptContext ------------------------------------------------------------------

ModelicaParser::SubscriptContext::SubscriptContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::ExpressionContext* ModelicaParser::SubscriptContext::expression() {
  return getRuleContext<ModelicaParser::ExpressionContext>(0);
}


size_t ModelicaParser::SubscriptContext::getRuleIndex() const {
  return ModelicaParser::RuleSubscript;
}


antlrcpp::Any ModelicaParser::SubscriptContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitSubscript(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::SubscriptContext* ModelicaParser::subscript() {
  SubscriptContext *_localctx = _tracker.createInstance<SubscriptContext>(_ctx, getState());
  enterRule(_localctx, 144, ModelicaParser::RuleSubscript);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(939);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ModelicaParser::T__17: {
        enterOuterAlt(_localctx, 1);
        setState(937);
        match(ModelicaParser::T__17);
        break;
      }

      case ModelicaParser::T__13:
      case ModelicaParser::T__16:
      case ModelicaParser::T__19:
      case ModelicaParser::T__39:
      case ModelicaParser::T__55:
      case ModelicaParser::T__56:
      case ModelicaParser::T__70:
      case ModelicaParser::T__73:
      case ModelicaParser::T__74:
      case ModelicaParser::T__75:
      case ModelicaParser::T__77:
      case ModelicaParser::INITIAL:
      case ModelicaParser::IDENT:
      case ModelicaParser::STRING:
      case ModelicaParser::UNSIGNED_NUMBER: {
        enterOuterAlt(_localctx, 2);
        setState(938);
        expression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CommentContext ------------------------------------------------------------------

ModelicaParser::CommentContext::CommentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::String_commentContext* ModelicaParser::CommentContext::string_comment() {
  return getRuleContext<ModelicaParser::String_commentContext>(0);
}

ModelicaParser::AnnotationContext* ModelicaParser::CommentContext::annotation() {
  return getRuleContext<ModelicaParser::AnnotationContext>(0);
}


size_t ModelicaParser::CommentContext::getRuleIndex() const {
  return ModelicaParser::RuleComment;
}


antlrcpp::Any ModelicaParser::CommentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitComment(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::CommentContext* ModelicaParser::comment() {
  CommentContext *_localctx = _tracker.createInstance<CommentContext>(_ctx, getState());
  enterRule(_localctx, 146, ModelicaParser::RuleComment);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(941);
    string_comment();
    setState(943);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::T__78) {
      setState(942);
      annotation();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- String_commentContext ------------------------------------------------------------------

ModelicaParser::String_commentContext::String_commentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ModelicaParser::String_commentContext::STRING() {
  return getTokens(ModelicaParser::STRING);
}

tree::TerminalNode* ModelicaParser::String_commentContext::STRING(size_t i) {
  return getToken(ModelicaParser::STRING, i);
}


size_t ModelicaParser::String_commentContext::getRuleIndex() const {
  return ModelicaParser::RuleString_comment;
}


antlrcpp::Any ModelicaParser::String_commentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitString_comment(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::String_commentContext* ModelicaParser::string_comment() {
  String_commentContext *_localctx = _tracker.createInstance<String_commentContext>(_ctx, getState());
  enterRule(_localctx, 148, ModelicaParser::RuleString_comment);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(953);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ModelicaParser::STRING) {
      setState(945);
      match(ModelicaParser::STRING);
      setState(950);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ModelicaParser::T__55) {
        setState(946);
        match(ModelicaParser::T__55);
        setState(947);
        match(ModelicaParser::STRING);
        setState(952);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AnnotationContext ------------------------------------------------------------------

ModelicaParser::AnnotationContext::AnnotationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ModelicaParser::Class_modificationContext* ModelicaParser::AnnotationContext::class_modification() {
  return getRuleContext<ModelicaParser::Class_modificationContext>(0);
}


size_t ModelicaParser::AnnotationContext::getRuleIndex() const {
  return ModelicaParser::RuleAnnotation;
}


antlrcpp::Any ModelicaParser::AnnotationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ModelicaVisitor*>(visitor))
    return parserVisitor->visitAnnotation(this);
  else
    return visitor->visitChildren(this);
}

ModelicaParser::AnnotationContext* ModelicaParser::annotation() {
  AnnotationContext *_localctx = _tracker.createInstance<AnnotationContext>(_ctx, getState());
  enterRule(_localctx, 150, ModelicaParser::RuleAnnotation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(955);
    match(ModelicaParser::T__78);
    setState(956);
    class_modification();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool ModelicaParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 59: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool ModelicaParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 7);
    case 1: return precpred(_ctx, 6);
    case 2: return precpred(_ctx, 5);
    case 3: return precpred(_ctx, 3);
    case 4: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> ModelicaParser::_decisionToDFA;
atn::PredictionContextCache ModelicaParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN ModelicaParser::_atn;
std::vector<uint16_t> ModelicaParser::_serializedATN;

std::vector<std::string> ModelicaParser::_ruleNames = {
  "stored_definition", "stored_definition_class", "class_definition", "class_prefixes", 
  "class_type", "class_specifier", "base_prefix", "enum_list", "enumeration_literal", 
  "composition", "language_specification", "external_function_call", "element_list", 
  "element", "regular_element", "replaceable_element", "import_clause", 
  "import_list", "extends_clause", "constraining_clause", "component_clause", 
  "type_prefix", "type_specifier_element", "type_specifier", "component_list", 
  "component_declaration", "condition_attribute", "declaration", "modification", 
  "class_modification", "argument_list", "argument", "element_modification_or_replaceable", 
  "element_modification", "element_redeclaration", "element_replaceable", 
  "component_clause1", "component_declaration1", "short_class_definition", 
  "equation_block", "equation_section", "statement_block", "algorithm_section", 
  "equation_options", "equation", "statement_options", "statement", "if_equation", 
  "if_statement", "for_equation", "for_statement", "for_indices", "for_index", 
  "while_statement", "when_equation", "when_statement", "connect_clause", 
  "expression", "simple_expression", "expr", "primary", "name", "component_reference_element", 
  "component_reference", "function_call_args", "function_arguments", "named_arguments", 
  "named_argument", "function_argument", "output_expression_list", "expression_list", 
  "array_subscripts", "subscript", "comment", "string_comment", "annotation"
};

std::vector<std::string> ModelicaParser::_literalNames = {
  "", "';'", "'class'", "'model'", "'operator'", "'record'", "'block'", 
  "'expandable'", "'connector'", "'type'", "'package'", "'pure'", "'impure'", 
  "'function'", "'end'", "'='", "'enumeration'", "'('", "':'", "')'", "'der'", 
  "','", "'extends'", "'public'", "'protected'", "'external'", "'replaceable'", 
  "'import'", "'.*'", "'.{'", "'}'", "'constrainedby'", "'flow'", "'stream'", 
  "'discrete'", "'parameter'", "'constant'", "'input'", "'output'", "'.'", 
  "'if'", "':='", "'equation'", "'algorithm'", "'break'", "'return'", "'then'", 
  "'elseif'", "'else'", "'for'", "'loop'", "'in'", "'while'", "'when'", 
  "'elsewhen'", "'connect'", "'+'", "'-'", "'^'", "'.^'", "'*'", "'/'", 
  "'./'", "'.+'", "'.-'", "'<'", "'<='", "'>'", "'>='", "'=='", "'<>'", 
  "'not'", "'and'", "'or'", "'false'", "'true'", "'['", "']'", "'{'", "'annotation'", 
  "'each'", "'partial'", "'final'", "'within'", "'encapsulated'", "'redeclare'", 
  "'inner'", "'outer'", "'initial'"
};

std::vector<std::string> ModelicaParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "EACH", "PARTIAL", "FINAL", "WITHIN", 
  "ENCAPSULATED", "REDECLARE", "INNER", "OUTER", "INITIAL", "IDENT", "STRING", 
  "UNSIGNED_NUMBER", "COMMENT", "WS"
};

dfa::Vocabulary ModelicaParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ModelicaParser::_tokenNames;

ModelicaParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x5f, 0x3c1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 
       0x9, 0x17, 0x4, 0x18, 0x9, 0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 
       0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 
       0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 0x1f, 0x9, 0x1f, 0x4, 0x20, 
       0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 0x9, 0x22, 0x4, 0x23, 
       0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 0x25, 0x4, 0x26, 
       0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 0x4, 0x29, 
       0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 0x2c, 
       0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
       0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 0x32, 
       0x9, 0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 
       0x9, 0x35, 0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x4, 0x38, 
       0x9, 0x38, 0x4, 0x39, 0x9, 0x39, 0x4, 0x3a, 0x9, 0x3a, 0x4, 0x3b, 
       0x9, 0x3b, 0x4, 0x3c, 0x9, 0x3c, 0x4, 0x3d, 0x9, 0x3d, 0x4, 0x3e, 
       0x9, 0x3e, 0x4, 0x3f, 0x9, 0x3f, 0x4, 0x40, 0x9, 0x40, 0x4, 0x41, 
       0x9, 0x41, 0x4, 0x42, 0x9, 0x42, 0x4, 0x43, 0x9, 0x43, 0x4, 0x44, 
       0x9, 0x44, 0x4, 0x45, 0x9, 0x45, 0x4, 0x46, 0x9, 0x46, 0x4, 0x47, 
       0x9, 0x47, 0x4, 0x48, 0x9, 0x48, 0x4, 0x49, 0x9, 0x49, 0x4, 0x4a, 
       0x9, 0x4a, 0x4, 0x4b, 0x9, 0x4b, 0x4, 0x4c, 0x9, 0x4c, 0x4, 0x4d, 
       0x9, 0x4d, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x9d, 0xa, 0x2, 0x3, 0x2, 
       0x5, 0x2, 0xa0, 0xa, 0x2, 0x3, 0x2, 0x7, 0x2, 0xa3, 0xa, 0x2, 0xc, 
       0x2, 0xe, 0x2, 0xa6, 0xb, 0x2, 0x3, 0x3, 0x5, 0x3, 0xa9, 0xa, 0x3, 
       0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x5, 0x4, 0xaf, 0xa, 0x4, 
       0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x5, 0x5, 0xb5, 0xa, 0x5, 
       0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0xbc, 
       0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0xc1, 0xa, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0xc7, 0xa, 0x6, 
       0x3, 0x6, 0x5, 0x6, 0xca, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 
       0xce, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 
       0x7, 0xdb, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
       0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0xe4, 0xa, 0x7, 0x3, 0x7, 0x5, 
       0x7, 0xe7, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
       0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
       0x7, 0x7, 0xf4, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0xf7, 0xb, 0x7, 0x3, 
       0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 
       0xff, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
       0x5, 0x7, 0x106, 0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 
       0x3, 0x9, 0x7, 0x9, 0x10d, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x110, 0xb, 
       0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
       0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x7, 0xb, 0x11c, 0xa, 0xb, 
       0xc, 0xb, 0xe, 0xb, 0x11f, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 
       0x123, 0xa, 0xb, 0x3, 0xb, 0x5, 0xb, 0x126, 0xa, 0xb, 0x3, 0xb, 0x5, 
       0xb, 0x129, 0xa, 0xb, 0x3, 0xb, 0x5, 0xb, 0x12c, 0xa, 0xb, 0x3, 0xb, 
       0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x131, 0xa, 0xb, 0x3, 0xc, 0x3, 0xc, 
       0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x138, 0xa, 0xd, 0x3, 0xd, 
       0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x13d, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 
       0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0x144, 0xa, 0xe, 0xc, 0xe, 
       0xe, 0xe, 0x147, 0xb, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
       0x5, 0xf, 0x14d, 0xa, 0xf, 0x3, 0x10, 0x5, 0x10, 0x150, 0xa, 0x10, 
       0x3, 0x10, 0x5, 0x10, 0x153, 0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0x156, 
       0xa, 0x10, 0x3, 0x10, 0x5, 0x10, 0x159, 0xa, 0x10, 0x3, 0x10, 0x3, 
       0x10, 0x5, 0x10, 0x15d, 0xa, 0x10, 0x3, 0x11, 0x5, 0x11, 0x160, 0xa, 
       0x11, 0x3, 0x11, 0x5, 0x11, 0x163, 0xa, 0x11, 0x3, 0x11, 0x5, 0x11, 
       0x166, 0xa, 0x11, 0x3, 0x11, 0x5, 0x11, 0x169, 0xa, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0x16e, 0xa, 0x11, 0x3, 0x11, 0x3, 
       0x11, 0x3, 0x11, 0x5, 0x11, 0x173, 0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 
       0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
       0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x17f, 0xa, 0x12, 0x5, 0x12, 0x181, 
       0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
       0x7, 0x13, 0x188, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x18b, 0xb, 0x13, 
       0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x190, 0xa, 0x14, 0x3, 
       0x14, 0x5, 0x14, 0x193, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
       0x5, 0x15, 0x198, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 
       0x16, 0x19d, 0xa, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x5, 0x17, 
       0x1a2, 0xa, 0x17, 0x3, 0x17, 0x5, 0x17, 0x1a5, 0xa, 0x17, 0x3, 0x17, 
       0x5, 0x17, 0x1a8, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 
       0x19, 0x3, 0x19, 0x7, 0x19, 0x1af, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 
       0x1b2, 0xb, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x7, 0x1a, 0x1b7, 
       0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 0x1ba, 0xb, 0x1a, 0x3, 0x1b, 0x3, 
       0x1b, 0x5, 0x1b, 0x1be, 0xa, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1c, 
       0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x1c7, 0xa, 
       0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x1ca, 0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 
       0x3, 0x1e, 0x5, 0x1e, 0x1cf, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 
       0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x1d5, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 
       0x5, 0x1f, 0x1d9, 0xa, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x20, 0x3, 
       0x20, 0x3, 0x20, 0x7, 0x20, 0x1e0, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 
       0x1e3, 0xb, 0x20, 0x3, 0x21, 0x3, 0x21, 0x5, 0x21, 0x1e7, 0xa, 0x21, 
       0x3, 0x22, 0x5, 0x22, 0x1ea, 0xa, 0x22, 0x3, 0x22, 0x5, 0x22, 0x1ed, 
       0xa, 0x22, 0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x1f1, 0xa, 0x22, 0x3, 
       0x23, 0x3, 0x23, 0x5, 0x23, 0x1f5, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 
       0x3, 0x24, 0x3, 0x24, 0x5, 0x24, 0x1fb, 0xa, 0x24, 0x3, 0x24, 0x5, 
       0x24, 0x1fe, 0xa, 0x24, 0x3, 0x24, 0x3, 0x24, 0x5, 0x24, 0x202, 0xa, 
       0x24, 0x3, 0x24, 0x5, 0x24, 0x205, 0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 
       0x3, 0x25, 0x5, 0x25, 0x20a, 0xa, 0x25, 0x3, 0x25, 0x5, 0x25, 0x20d, 
       0xa, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 
       0x3, 0x27, 0x3, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 
       0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x21c, 0xa, 0x28, 0x3, 0x28, 0x5, 
       0x28, 0x21f, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 
       0x3, 0x28, 0x5, 0x28, 0x226, 0xa, 0x28, 0x3, 0x28, 0x5, 0x28, 0x229, 
       0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0x22d, 0xa, 0x28, 0x3, 
       0x29, 0x3, 0x29, 0x3, 0x29, 0x7, 0x29, 0x232, 0xa, 0x29, 0xc, 0x29, 
       0xe, 0x29, 0x235, 0xb, 0x29, 0x3, 0x2a, 0x5, 0x2a, 0x238, 0xa, 0x2a, 
       0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
       0x7, 0x2b, 0x240, 0xa, 0x2b, 0xc, 0x2b, 0xe, 0x2b, 0x243, 0xb, 0x2b, 
       0x3, 0x2c, 0x5, 0x2c, 0x246, 0xa, 0x2c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 
       0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 
       0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x5, 
       0x2d, 0x256, 0xa, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 
       0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x5, 0x2f, 0x25f, 0xa, 0x2f, 0x3, 
       0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x7, 0x2f, 0x265, 0xa, 0x2f, 
       0xc, 0x2f, 0xe, 0x2f, 0x268, 0xb, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 
       0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 
       0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x5, 0x2f, 0x275, 0xa, 0x2f, 0x3, 0x30, 
       0x3, 0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
       0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x7, 0x31, 
       0x283, 0xa, 0x31, 0xc, 0x31, 0xe, 0x31, 0x286, 0xb, 0x31, 0x3, 0x31, 
       0x3, 0x31, 0x5, 0x31, 0x28a, 0xa, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 
       0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 
       0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x7, 0x32, 0x298, 0xa, 0x32, 
       0xc, 0x32, 0xe, 0x32, 0x29b, 0xb, 0x32, 0x3, 0x32, 0x3, 0x32, 0x5, 
       0x32, 0x29f, 0xa, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x33, 
       0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 
       0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 
       0x3, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x7, 0x35, 0x2b5, 0xa, 
       0x35, 0xc, 0x35, 0xe, 0x35, 0x2b8, 0xb, 0x35, 0x3, 0x36, 0x3, 0x36, 
       0x3, 0x36, 0x5, 0x36, 0x2bd, 0xa, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 
       0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x38, 0x3, 
       0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 
       0x38, 0x3, 0x38, 0x7, 0x38, 0x2cf, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 
       0x2d2, 0xb, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x39, 0x3, 
       0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 
       0x39, 0x3, 0x39, 0x7, 0x39, 0x2e0, 0xa, 0x39, 0xc, 0x39, 0xe, 0x39, 
       0x2e3, 0xb, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x3, 0x3a, 0x3, 
       0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 0x3a, 0x3, 
       0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 
       0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x7, 0x3b, 0x2f9, 0xa, 0x3b, 
       0xc, 0x3b, 0xe, 0x3b, 0x2fc, 0xb, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 0x3, 
       0x3b, 0x5, 0x3b, 0x301, 0xa, 0x3b, 0x3, 0x3c, 0x3, 0x3c, 0x3, 0x3c, 
       0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x308, 0xa, 0x3c, 0x5, 0x3c, 0x30a, 
       0xa, 0x3c, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 
       0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x5, 0x3d, 
       0x316, 0xa, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
       0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 
       0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3d, 0x7, 0x3d, 0x327, 
       0xa, 0x3d, 0xc, 0x3d, 0xe, 0x3d, 0x32a, 0xb, 0x3d, 0x3, 0x3e, 0x3, 
       0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
       0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
       0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
       0x3e, 0x7, 0x3e, 0x340, 0xa, 0x3e, 0xc, 0x3e, 0xe, 0x3e, 0x343, 0xb, 
       0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
       0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x34c, 0xa, 0x3e, 0x3, 0x3f, 0x5, 0x3f, 
       0x34f, 0xa, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x7, 0x3f, 0x354, 
       0xa, 0x3f, 0xc, 0x3f, 0xe, 0x3f, 0x357, 0xb, 0x3f, 0x3, 0x40, 0x3, 
       0x40, 0x5, 0x40, 0x35b, 0xa, 0x40, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 
       0x7, 0x41, 0x360, 0xa, 0x41, 0xc, 0x41, 0xe, 0x41, 0x363, 0xb, 0x41, 
       0x3, 0x42, 0x3, 0x42, 0x5, 0x42, 0x367, 0xa, 0x42, 0x3, 0x42, 0x3, 
       0x42, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x3, 0x43, 0x7, 
       0x43, 0x370, 0xa, 0x43, 0xc, 0x43, 0xe, 0x43, 0x373, 0xb, 0x43, 0x3, 
       0x43, 0x5, 0x43, 0x376, 0xa, 0x43, 0x3, 0x44, 0x3, 0x44, 0x3, 0x44, 
       0x7, 0x44, 0x37b, 0xa, 0x44, 0xc, 0x44, 0xe, 0x44, 0x37e, 0xb, 0x44, 
       0x3, 0x45, 0x3, 0x45, 0x3, 0x45, 0x3, 0x45, 0x3, 0x46, 0x3, 0x46, 
       0x3, 0x46, 0x3, 0x46, 0x5, 0x46, 0x388, 0xa, 0x46, 0x3, 0x46, 0x3, 
       0x46, 0x3, 0x46, 0x5, 0x46, 0x38d, 0xa, 0x46, 0x3, 0x47, 0x5, 0x47, 
       0x390, 0xa, 0x47, 0x3, 0x47, 0x3, 0x47, 0x7, 0x47, 0x394, 0xa, 0x47, 
       0xc, 0x47, 0xe, 0x47, 0x397, 0xb, 0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 
       0x48, 0x7, 0x48, 0x39c, 0xa, 0x48, 0xc, 0x48, 0xe, 0x48, 0x39f, 0xb, 
       0x48, 0x3, 0x49, 0x3, 0x49, 0x3, 0x49, 0x3, 0x49, 0x7, 0x49, 0x3a5, 
       0xa, 0x49, 0xc, 0x49, 0xe, 0x49, 0x3a8, 0xb, 0x49, 0x3, 0x49, 0x3, 
       0x49, 0x3, 0x4a, 0x3, 0x4a, 0x5, 0x4a, 0x3ae, 0xa, 0x4a, 0x3, 0x4b, 
       0x3, 0x4b, 0x5, 0x4b, 0x3b2, 0xa, 0x4b, 0x3, 0x4c, 0x3, 0x4c, 0x3, 
       0x4c, 0x7, 0x4c, 0x3b7, 0xa, 0x4c, 0xc, 0x4c, 0xe, 0x4c, 0x3ba, 0xb, 
       0x4c, 0x5, 0x4c, 0x3bc, 0xa, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 
       0x3, 0x4d, 0x2, 0x3, 0x78, 0x4e, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 
       0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 
       0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 
       0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 
       0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x64, 0x66, 
       0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 
       0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 
       0x94, 0x96, 0x98, 0x2, 0xb, 0x3, 0x2, 0xd, 0xe, 0x3, 0x2, 0x22, 0x23, 
       0x3, 0x2, 0x24, 0x26, 0x3, 0x2, 0x27, 0x28, 0x3, 0x2, 0x3a, 0x3b, 
       0x3, 0x2, 0x3c, 0x3d, 0x4, 0x2, 0x1e, 0x1e, 0x3e, 0x40, 0x4, 0x2, 
       0x3a, 0x3b, 0x41, 0x42, 0x3, 0x2, 0x43, 0x48, 0x2, 0x414, 0x2, 0x9f, 
       0x3, 0x2, 0x2, 0x2, 0x4, 0xa8, 0x3, 0x2, 0x2, 0x2, 0x6, 0xae, 0x3, 
       0x2, 0x2, 0x2, 0x8, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xa, 0xcd, 0x3, 0x2, 
       0x2, 0x2, 0xc, 0x105, 0x3, 0x2, 0x2, 0x2, 0xe, 0x107, 0x3, 0x2, 0x2, 
       0x2, 0x10, 0x109, 0x3, 0x2, 0x2, 0x2, 0x12, 0x111, 0x3, 0x2, 0x2, 
       0x2, 0x14, 0x114, 0x3, 0x2, 0x2, 0x2, 0x16, 0x132, 0x3, 0x2, 0x2, 
       0x2, 0x18, 0x137, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x145, 0x3, 0x2, 0x2, 
       0x2, 0x1c, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x14f, 0x3, 0x2, 0x2, 
       0x2, 0x20, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x22, 0x174, 0x3, 0x2, 0x2, 
       0x2, 0x24, 0x184, 0x3, 0x2, 0x2, 0x2, 0x26, 0x18c, 0x3, 0x2, 0x2, 
       0x2, 0x28, 0x194, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x199, 0x3, 0x2, 0x2, 
       0x2, 0x2c, 0x1a1, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x1a9, 0x3, 0x2, 0x2, 
       0x2, 0x30, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x32, 0x1b3, 0x3, 0x2, 0x2, 
       0x2, 0x34, 0x1bb, 0x3, 0x2, 0x2, 0x2, 0x36, 0x1c1, 0x3, 0x2, 0x2, 
       0x2, 0x38, 0x1c4, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x1d4, 0x3, 0x2, 0x2, 
       0x2, 0x3c, 0x1d6, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x1dc, 0x3, 0x2, 0x2, 
       0x2, 0x40, 0x1e6, 0x3, 0x2, 0x2, 0x2, 0x42, 0x1e9, 0x3, 0x2, 0x2, 
       0x2, 0x44, 0x1f2, 0x3, 0x2, 0x2, 0x2, 0x46, 0x1f8, 0x3, 0x2, 0x2, 
       0x2, 0x48, 0x206, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x20e, 0x3, 0x2, 0x2, 
       0x2, 0x4c, 0x212, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x215, 0x3, 0x2, 0x2, 
       0x2, 0x50, 0x233, 0x3, 0x2, 0x2, 0x2, 0x52, 0x237, 0x3, 0x2, 0x2, 
       0x2, 0x54, 0x241, 0x3, 0x2, 0x2, 0x2, 0x56, 0x245, 0x3, 0x2, 0x2, 
       0x2, 0x58, 0x255, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x257, 0x3, 0x2, 0x2, 
       0x2, 0x5c, 0x274, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x276, 0x3, 0x2, 0x2, 
       0x2, 0x60, 0x279, 0x3, 0x2, 0x2, 0x2, 0x62, 0x28e, 0x3, 0x2, 0x2, 
       0x2, 0x64, 0x2a3, 0x3, 0x2, 0x2, 0x2, 0x66, 0x2aa, 0x3, 0x2, 0x2, 
       0x2, 0x68, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x2b9, 0x3, 0x2, 0x2, 
       0x2, 0x6c, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x2c5, 0x3, 0x2, 0x2, 
       0x2, 0x70, 0x2d6, 0x3, 0x2, 0x2, 0x2, 0x72, 0x2e7, 0x3, 0x2, 0x2, 
       0x2, 0x74, 0x300, 0x3, 0x2, 0x2, 0x2, 0x76, 0x302, 0x3, 0x2, 0x2, 
       0x2, 0x78, 0x315, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x34b, 0x3, 0x2, 0x2, 
       0x2, 0x7c, 0x34e, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x358, 0x3, 0x2, 0x2, 
       0x2, 0x80, 0x35c, 0x3, 0x2, 0x2, 0x2, 0x82, 0x364, 0x3, 0x2, 0x2, 
       0x2, 0x84, 0x375, 0x3, 0x2, 0x2, 0x2, 0x86, 0x377, 0x3, 0x2, 0x2, 
       0x2, 0x88, 0x37f, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x38c, 0x3, 0x2, 0x2, 
       0x2, 0x8c, 0x38f, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x398, 0x3, 0x2, 0x2, 
       0x2, 0x90, 0x3a0, 0x3, 0x2, 0x2, 0x2, 0x92, 0x3ad, 0x3, 0x2, 0x2, 
       0x2, 0x94, 0x3af, 0x3, 0x2, 0x2, 0x2, 0x96, 0x3bb, 0x3, 0x2, 0x2, 
       0x2, 0x98, 0x3bd, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9c, 0x7, 0x55, 0x2, 
       0x2, 0x9b, 0x9d, 0x5, 0x80, 0x41, 0x2, 0x9c, 0x9b, 0x3, 0x2, 0x2, 
       0x2, 0x9c, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9e, 0x3, 0x2, 0x2, 0x2, 
       0x9e, 0xa0, 0x7, 0x3, 0x2, 0x2, 0x9f, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x9f, 
       0xa0, 0x3, 0x2, 0x2, 0x2, 0xa0, 0xa4, 0x3, 0x2, 0x2, 0x2, 0xa1, 0xa3, 
       0x5, 0x4, 0x3, 0x2, 0xa2, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa6, 0x3, 
       0x2, 0x2, 0x2, 0xa4, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x3, 0x2, 
       0x2, 0x2, 0xa5, 0x3, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa4, 0x3, 0x2, 0x2, 
       0x2, 0xa7, 0xa9, 0x7, 0x54, 0x2, 0x2, 0xa8, 0xa7, 0x3, 0x2, 0x2, 
       0x2, 0xa8, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xaa, 0x3, 0x2, 0x2, 0x2, 
       0xaa, 0xab, 0x5, 0x6, 0x4, 0x2, 0xab, 0xac, 0x7, 0x3, 0x2, 0x2, 0xac, 
       0x5, 0x3, 0x2, 0x2, 0x2, 0xad, 0xaf, 0x7, 0x56, 0x2, 0x2, 0xae, 0xad, 
       0x3, 0x2, 0x2, 0x2, 0xae, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xb0, 0x3, 
       0x2, 0x2, 0x2, 0xb0, 0xb1, 0x5, 0x8, 0x5, 0x2, 0xb1, 0xb2, 0x5, 0xc, 
       0x7, 0x2, 0xb2, 0x7, 0x3, 0x2, 0x2, 0x2, 0xb3, 0xb5, 0x7, 0x53, 0x2, 
       0x2, 0xb4, 0xb3, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb5, 0x3, 0x2, 0x2, 0x2, 
       0xb5, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb7, 0x5, 0xa, 0x6, 0x2, 0xb7, 
       0x9, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xce, 0x7, 0x4, 0x2, 0x2, 0xb9, 0xce, 
       0x7, 0x5, 0x2, 0x2, 0xba, 0xbc, 0x7, 0x6, 0x2, 0x2, 0xbb, 0xba, 0x3, 
       0x2, 0x2, 0x2, 0xbb, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xbd, 0x3, 0x2, 
       0x2, 0x2, 0xbd, 0xce, 0x7, 0x7, 0x2, 0x2, 0xbe, 0xce, 0x7, 0x8, 0x2, 
       0x2, 0xbf, 0xc1, 0x7, 0x9, 0x2, 0x2, 0xc0, 0xbf, 0x3, 0x2, 0x2, 0x2, 
       0xc0, 0xc1, 0x3, 0x2, 0x2, 0x2, 0xc1, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc2, 
       0xce, 0x7, 0xa, 0x2, 0x2, 0xc3, 0xce, 0x7, 0xb, 0x2, 0x2, 0xc4, 0xce, 
       0x7, 0xc, 0x2, 0x2, 0xc5, 0xc7, 0x9, 0x2, 0x2, 0x2, 0xc6, 0xc5, 0x3, 
       0x2, 0x2, 0x2, 0xc6, 0xc7, 0x3, 0x2, 0x2, 0x2, 0xc7, 0xc9, 0x3, 0x2, 
       0x2, 0x2, 0xc8, 0xca, 0x7, 0x6, 0x2, 0x2, 0xc9, 0xc8, 0x3, 0x2, 0x2, 
       0x2, 0xc9, 0xca, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcb, 0x3, 0x2, 0x2, 0x2, 
       0xcb, 0xce, 0x7, 0xf, 0x2, 0x2, 0xcc, 0xce, 0x7, 0x6, 0x2, 0x2, 0xcd, 
       0xb8, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xb9, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xbb, 
       0x3, 0x2, 0x2, 0x2, 0xcd, 0xbe, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xc0, 0x3, 
       0x2, 0x2, 0x2, 0xcd, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xc4, 0x3, 0x2, 
       0x2, 0x2, 0xcd, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xcc, 0x3, 0x2, 0x2, 
       0x2, 0xce, 0xb, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xd0, 0x7, 0x5b, 0x2, 0x2, 
       0xd0, 0xd1, 0x5, 0x96, 0x4c, 0x2, 0xd1, 0xd2, 0x5, 0x14, 0xb, 0x2, 
       0xd2, 0xd3, 0x7, 0x10, 0x2, 0x2, 0xd3, 0xd4, 0x7, 0x5b, 0x2, 0x2, 
       0xd4, 0x106, 0x3, 0x2, 0x2, 0x2, 0xd5, 0xd6, 0x7, 0x5b, 0x2, 0x2, 
       0xd6, 0xd7, 0x7, 0x11, 0x2, 0x2, 0xd7, 0xd8, 0x5, 0xe, 0x8, 0x2, 
       0xd8, 0xda, 0x5, 0x80, 0x41, 0x2, 0xd9, 0xdb, 0x5, 0x3c, 0x1f, 0x2, 
       0xda, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdb, 
       0xdc, 0x3, 0x2, 0x2, 0x2, 0xdc, 0xdd, 0x5, 0x94, 0x4b, 0x2, 0xdd, 
       0x106, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdf, 0x7, 0x5b, 0x2, 0x2, 0xdf, 
       0xe0, 0x7, 0x11, 0x2, 0x2, 0xe0, 0xe1, 0x7, 0x12, 0x2, 0x2, 0xe1, 
       0xe6, 0x7, 0x13, 0x2, 0x2, 0xe2, 0xe4, 0x5, 0x10, 0x9, 0x2, 0xe3, 
       0xe2, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 0x3, 0x2, 0x2, 0x2, 0xe4, 0xe7, 
       0x3, 0x2, 0x2, 0x2, 0xe5, 0xe7, 0x7, 0x14, 0x2, 0x2, 0xe6, 0xe3, 
       0x3, 0x2, 0x2, 0x2, 0xe6, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe8, 0x3, 
       0x2, 0x2, 0x2, 0xe8, 0xe9, 0x7, 0x15, 0x2, 0x2, 0xe9, 0x106, 0x5, 
       0x94, 0x4b, 0x2, 0xea, 0xeb, 0x7, 0x5b, 0x2, 0x2, 0xeb, 0xec, 0x7, 
       0x11, 0x2, 0x2, 0xec, 0xed, 0x7, 0x16, 0x2, 0x2, 0xed, 0xee, 0x7, 
       0x13, 0x2, 0x2, 0xee, 0xef, 0x5, 0x7c, 0x3f, 0x2, 0xef, 0xf0, 0x7, 
       0x17, 0x2, 0x2, 0xf0, 0xf5, 0x7, 0x5b, 0x2, 0x2, 0xf1, 0xf2, 0x7, 
       0x17, 0x2, 0x2, 0xf2, 0xf4, 0x7, 0x5b, 0x2, 0x2, 0xf3, 0xf1, 0x3, 
       0x2, 0x2, 0x2, 0xf4, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xf5, 0xf3, 0x3, 0x2, 
       0x2, 0x2, 0xf5, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xf8, 0x3, 0x2, 0x2, 
       0x2, 0xf7, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xf9, 0x7, 0x15, 0x2, 
       0x2, 0xf9, 0xfa, 0x5, 0x94, 0x4b, 0x2, 0xfa, 0x106, 0x3, 0x2, 0x2, 
       0x2, 0xfb, 0xfc, 0x7, 0x18, 0x2, 0x2, 0xfc, 0xfe, 0x7, 0x5b, 0x2, 
       0x2, 0xfd, 0xff, 0x5, 0x3c, 0x1f, 0x2, 0xfe, 0xfd, 0x3, 0x2, 0x2, 
       0x2, 0xfe, 0xff, 0x3, 0x2, 0x2, 0x2, 0xff, 0x100, 0x3, 0x2, 0x2, 
       0x2, 0x100, 0x101, 0x5, 0x96, 0x4c, 0x2, 0x101, 0x102, 0x5, 0x14, 
       0xb, 0x2, 0x102, 0x103, 0x7, 0x10, 0x2, 0x2, 0x103, 0x104, 0x7, 0x5b, 
       0x2, 0x2, 0x104, 0x106, 0x3, 0x2, 0x2, 0x2, 0x105, 0xcf, 0x3, 0x2, 
       0x2, 0x2, 0x105, 0xd5, 0x3, 0x2, 0x2, 0x2, 0x105, 0xde, 0x3, 0x2, 
       0x2, 0x2, 0x105, 0xea, 0x3, 0x2, 0x2, 0x2, 0x105, 0xfb, 0x3, 0x2, 
       0x2, 0x2, 0x106, 0xd, 0x3, 0x2, 0x2, 0x2, 0x107, 0x108, 0x5, 0x2c, 
       0x17, 0x2, 0x108, 0xf, 0x3, 0x2, 0x2, 0x2, 0x109, 0x10e, 0x5, 0x12, 
       0xa, 0x2, 0x10a, 0x10b, 0x7, 0x17, 0x2, 0x2, 0x10b, 0x10d, 0x5, 0x12, 
       0xa, 0x2, 0x10c, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x110, 0x3, 0x2, 
       0x2, 0x2, 0x10e, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10f, 0x3, 0x2, 
       0x2, 0x2, 0x10f, 0x11, 0x3, 0x2, 0x2, 0x2, 0x110, 0x10e, 0x3, 0x2, 
       0x2, 0x2, 0x111, 0x112, 0x7, 0x5b, 0x2, 0x2, 0x112, 0x113, 0x5, 0x94, 
       0x4b, 0x2, 0x113, 0x13, 0x3, 0x2, 0x2, 0x2, 0x114, 0x11d, 0x5, 0x1a, 
       0xe, 0x2, 0x115, 0x116, 0x7, 0x19, 0x2, 0x2, 0x116, 0x11c, 0x5, 0x1a, 
       0xe, 0x2, 0x117, 0x118, 0x7, 0x1a, 0x2, 0x2, 0x118, 0x11c, 0x5, 0x1a, 
       0xe, 0x2, 0x119, 0x11c, 0x5, 0x52, 0x2a, 0x2, 0x11a, 0x11c, 0x5, 
       0x56, 0x2c, 0x2, 0x11b, 0x115, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x117, 
       0x3, 0x2, 0x2, 0x2, 0x11b, 0x119, 0x3, 0x2, 0x2, 0x2, 0x11b, 0x11a, 
       0x3, 0x2, 0x2, 0x2, 0x11c, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x11d, 0x11b, 
       0x3, 0x2, 0x2, 0x2, 0x11d, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x11e, 0x12b, 
       0x3, 0x2, 0x2, 0x2, 0x11f, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x120, 0x122, 
       0x7, 0x1b, 0x2, 0x2, 0x121, 0x123, 0x5, 0x16, 0xc, 0x2, 0x122, 0x121, 
       0x3, 0x2, 0x2, 0x2, 0x122, 0x123, 0x3, 0x2, 0x2, 0x2, 0x123, 0x125, 
       0x3, 0x2, 0x2, 0x2, 0x124, 0x126, 0x5, 0x18, 0xd, 0x2, 0x125, 0x124, 
       0x3, 0x2, 0x2, 0x2, 0x125, 0x126, 0x3, 0x2, 0x2, 0x2, 0x126, 0x128, 
       0x3, 0x2, 0x2, 0x2, 0x127, 0x129, 0x5, 0x98, 0x4d, 0x2, 0x128, 0x127, 
       0x3, 0x2, 0x2, 0x2, 0x128, 0x129, 0x3, 0x2, 0x2, 0x2, 0x129, 0x12a, 
       0x3, 0x2, 0x2, 0x2, 0x12a, 0x12c, 0x7, 0x3, 0x2, 0x2, 0x12b, 0x120, 
       0x3, 0x2, 0x2, 0x2, 0x12b, 0x12c, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x130, 
       0x3, 0x2, 0x2, 0x2, 0x12d, 0x12e, 0x5, 0x98, 0x4d, 0x2, 0x12e, 0x12f, 
       0x7, 0x3, 0x2, 0x2, 0x12f, 0x131, 0x3, 0x2, 0x2, 0x2, 0x130, 0x12d, 
       0x3, 0x2, 0x2, 0x2, 0x130, 0x131, 0x3, 0x2, 0x2, 0x2, 0x131, 0x15, 
       0x3, 0x2, 0x2, 0x2, 0x132, 0x133, 0x7, 0x5c, 0x2, 0x2, 0x133, 0x17, 
       0x3, 0x2, 0x2, 0x2, 0x134, 0x135, 0x5, 0x80, 0x41, 0x2, 0x135, 0x136, 
       0x7, 0x11, 0x2, 0x2, 0x136, 0x138, 0x3, 0x2, 0x2, 0x2, 0x137, 0x134, 
       0x3, 0x2, 0x2, 0x2, 0x137, 0x138, 0x3, 0x2, 0x2, 0x2, 0x138, 0x139, 
       0x3, 0x2, 0x2, 0x2, 0x139, 0x13a, 0x7, 0x5b, 0x2, 0x2, 0x13a, 0x13c, 
       0x7, 0x13, 0x2, 0x2, 0x13b, 0x13d, 0x5, 0x8e, 0x48, 0x2, 0x13c, 0x13b, 
       0x3, 0x2, 0x2, 0x2, 0x13c, 0x13d, 0x3, 0x2, 0x2, 0x2, 0x13d, 0x13e, 
       0x3, 0x2, 0x2, 0x2, 0x13e, 0x13f, 0x7, 0x15, 0x2, 0x2, 0x13f, 0x19, 
       0x3, 0x2, 0x2, 0x2, 0x140, 0x141, 0x5, 0x1c, 0xf, 0x2, 0x141, 0x142, 
       0x7, 0x3, 0x2, 0x2, 0x142, 0x144, 0x3, 0x2, 0x2, 0x2, 0x143, 0x140, 
       0x3, 0x2, 0x2, 0x2, 0x144, 0x147, 0x3, 0x2, 0x2, 0x2, 0x145, 0x143, 
       0x3, 0x2, 0x2, 0x2, 0x145, 0x146, 0x3, 0x2, 0x2, 0x2, 0x146, 0x1b, 
       0x3, 0x2, 0x2, 0x2, 0x147, 0x145, 0x3, 0x2, 0x2, 0x2, 0x148, 0x14d, 
       0x5, 0x22, 0x12, 0x2, 0x149, 0x14d, 0x5, 0x26, 0x14, 0x2, 0x14a, 
       0x14d, 0x5, 0x1e, 0x10, 0x2, 0x14b, 0x14d, 0x5, 0x20, 0x11, 0x2, 
       0x14c, 0x148, 0x3, 0x2, 0x2, 0x2, 0x14c, 0x149, 0x3, 0x2, 0x2, 0x2, 
       0x14c, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x14c, 0x14b, 0x3, 0x2, 0x2, 0x2, 
       0x14d, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x14e, 0x150, 0x7, 0x57, 0x2, 0x2, 
       0x14f, 0x14e, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x150, 0x3, 0x2, 0x2, 0x2, 
       0x150, 0x152, 0x3, 0x2, 0x2, 0x2, 0x151, 0x153, 0x7, 0x54, 0x2, 0x2, 
       0x152, 0x151, 0x3, 0x2, 0x2, 0x2, 0x152, 0x153, 0x3, 0x2, 0x2, 0x2, 
       0x153, 0x155, 0x3, 0x2, 0x2, 0x2, 0x154, 0x156, 0x7, 0x58, 0x2, 0x2, 
       0x155, 0x154, 0x3, 0x2, 0x2, 0x2, 0x155, 0x156, 0x3, 0x2, 0x2, 0x2, 
       0x156, 0x158, 0x3, 0x2, 0x2, 0x2, 0x157, 0x159, 0x7, 0x59, 0x2, 0x2, 
       0x158, 0x157, 0x3, 0x2, 0x2, 0x2, 0x158, 0x159, 0x3, 0x2, 0x2, 0x2, 
       0x159, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15d, 0x5, 0x6, 0x4, 0x2, 
       0x15b, 0x15d, 0x5, 0x2a, 0x16, 0x2, 0x15c, 0x15a, 0x3, 0x2, 0x2, 
       0x2, 0x15c, 0x15b, 0x3, 0x2, 0x2, 0x2, 0x15d, 0x1f, 0x3, 0x2, 0x2, 
       0x2, 0x15e, 0x160, 0x7, 0x57, 0x2, 0x2, 0x15f, 0x15e, 0x3, 0x2, 0x2, 
       0x2, 0x15f, 0x160, 0x3, 0x2, 0x2, 0x2, 0x160, 0x162, 0x3, 0x2, 0x2, 
       0x2, 0x161, 0x163, 0x7, 0x54, 0x2, 0x2, 0x162, 0x161, 0x3, 0x2, 0x2, 
       0x2, 0x162, 0x163, 0x3, 0x2, 0x2, 0x2, 0x163, 0x165, 0x3, 0x2, 0x2, 
       0x2, 0x164, 0x166, 0x7, 0x58, 0x2, 0x2, 0x165, 0x164, 0x3, 0x2, 0x2, 
       0x2, 0x165, 0x166, 0x3, 0x2, 0x2, 0x2, 0x166, 0x168, 0x3, 0x2, 0x2, 
       0x2, 0x167, 0x169, 0x7, 0x59, 0x2, 0x2, 0x168, 0x167, 0x3, 0x2, 0x2, 
       0x2, 0x168, 0x169, 0x3, 0x2, 0x2, 0x2, 0x169, 0x16a, 0x3, 0x2, 0x2, 
       0x2, 0x16a, 0x16d, 0x7, 0x1c, 0x2, 0x2, 0x16b, 0x16e, 0x5, 0x6, 0x4, 
       0x2, 0x16c, 0x16e, 0x5, 0x2a, 0x16, 0x2, 0x16d, 0x16b, 0x3, 0x2, 
       0x2, 0x2, 0x16d, 0x16c, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x172, 0x3, 0x2, 
       0x2, 0x2, 0x16f, 0x170, 0x5, 0x28, 0x15, 0x2, 0x170, 0x171, 0x5, 
       0x94, 0x4b, 0x2, 0x171, 0x173, 0x3, 0x2, 0x2, 0x2, 0x172, 0x16f, 
       0x3, 0x2, 0x2, 0x2, 0x172, 0x173, 0x3, 0x2, 0x2, 0x2, 0x173, 0x21, 
       0x3, 0x2, 0x2, 0x2, 0x174, 0x180, 0x7, 0x1d, 0x2, 0x2, 0x175, 0x176, 
       0x7, 0x5b, 0x2, 0x2, 0x176, 0x177, 0x7, 0x11, 0x2, 0x2, 0x177, 0x181, 
       0x5, 0x80, 0x41, 0x2, 0x178, 0x17e, 0x5, 0x80, 0x41, 0x2, 0x179, 
       0x17f, 0x7, 0x1e, 0x2, 0x2, 0x17a, 0x17b, 0x7, 0x1f, 0x2, 0x2, 0x17b, 
       0x17c, 0x5, 0x24, 0x13, 0x2, 0x17c, 0x17d, 0x7, 0x20, 0x2, 0x2, 0x17d, 
       0x17f, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x179, 0x3, 0x2, 0x2, 0x2, 0x17e, 
       0x17a, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x17f, 0x3, 0x2, 0x2, 0x2, 0x17f, 
       0x181, 0x3, 0x2, 0x2, 0x2, 0x180, 0x175, 0x3, 0x2, 0x2, 0x2, 0x180, 
       0x178, 0x3, 0x2, 0x2, 0x2, 0x181, 0x182, 0x3, 0x2, 0x2, 0x2, 0x182, 
       0x183, 0x5, 0x94, 0x4b, 0x2, 0x183, 0x23, 0x3, 0x2, 0x2, 0x2, 0x184, 
       0x189, 0x7, 0x5b, 0x2, 0x2, 0x185, 0x186, 0x7, 0x17, 0x2, 0x2, 0x186, 
       0x188, 0x5, 0x24, 0x13, 0x2, 0x187, 0x185, 0x3, 0x2, 0x2, 0x2, 0x188, 
       0x18b, 0x3, 0x2, 0x2, 0x2, 0x189, 0x187, 0x3, 0x2, 0x2, 0x2, 0x189, 
       0x18a, 0x3, 0x2, 0x2, 0x2, 0x18a, 0x25, 0x3, 0x2, 0x2, 0x2, 0x18b, 
       0x189, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18d, 0x7, 0x18, 0x2, 0x2, 0x18d, 
       0x18f, 0x5, 0x80, 0x41, 0x2, 0x18e, 0x190, 0x5, 0x3c, 0x1f, 0x2, 
       0x18f, 0x18e, 0x3, 0x2, 0x2, 0x2, 0x18f, 0x190, 0x3, 0x2, 0x2, 0x2, 
       0x190, 0x192, 0x3, 0x2, 0x2, 0x2, 0x191, 0x193, 0x5, 0x98, 0x4d, 
       0x2, 0x192, 0x191, 0x3, 0x2, 0x2, 0x2, 0x192, 0x193, 0x3, 0x2, 0x2, 
       0x2, 0x193, 0x27, 0x3, 0x2, 0x2, 0x2, 0x194, 0x195, 0x7, 0x21, 0x2, 
       0x2, 0x195, 0x197, 0x5, 0x7c, 0x3f, 0x2, 0x196, 0x198, 0x5, 0x3c, 
       0x1f, 0x2, 0x197, 0x196, 0x3, 0x2, 0x2, 0x2, 0x197, 0x198, 0x3, 0x2, 
       0x2, 0x2, 0x198, 0x29, 0x3, 0x2, 0x2, 0x2, 0x199, 0x19a, 0x5, 0x2c, 
       0x17, 0x2, 0x19a, 0x19c, 0x5, 0x30, 0x19, 0x2, 0x19b, 0x19d, 0x5, 
       0x90, 0x49, 0x2, 0x19c, 0x19b, 0x3, 0x2, 0x2, 0x2, 0x19c, 0x19d, 
       0x3, 0x2, 0x2, 0x2, 0x19d, 0x19e, 0x3, 0x2, 0x2, 0x2, 0x19e, 0x19f, 
       0x5, 0x32, 0x1a, 0x2, 0x19f, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x1a0, 0x1a2, 
       0x9, 0x3, 0x2, 0x2, 0x1a1, 0x1a0, 0x3, 0x2, 0x2, 0x2, 0x1a1, 0x1a2, 
       0x3, 0x2, 0x2, 0x2, 0x1a2, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x1a3, 0x1a5, 
       0x9, 0x4, 0x2, 0x2, 0x1a4, 0x1a3, 0x3, 0x2, 0x2, 0x2, 0x1a4, 0x1a5, 
       0x3, 0x2, 0x2, 0x2, 0x1a5, 0x1a7, 0x3, 0x2, 0x2, 0x2, 0x1a6, 0x1a8, 
       0x9, 0x5, 0x2, 0x2, 0x1a7, 0x1a6, 0x3, 0x2, 0x2, 0x2, 0x1a7, 0x1a8, 
       0x3, 0x2, 0x2, 0x2, 0x1a8, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x1a9, 0x1aa, 
       0x7, 0x5b, 0x2, 0x2, 0x1aa, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x1b0, 
       0x5, 0x2e, 0x18, 0x2, 0x1ac, 0x1ad, 0x7, 0x29, 0x2, 0x2, 0x1ad, 0x1af, 
       0x5, 0x2e, 0x18, 0x2, 0x1ae, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1af, 0x1b2, 
       0x3, 0x2, 0x2, 0x2, 0x1b0, 0x1ae, 0x3, 0x2, 0x2, 0x2, 0x1b0, 0x1b1, 
       0x3, 0x2, 0x2, 0x2, 0x1b1, 0x31, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1b0, 
       0x3, 0x2, 0x2, 0x2, 0x1b3, 0x1b8, 0x5, 0x34, 0x1b, 0x2, 0x1b4, 0x1b5, 
       0x7, 0x17, 0x2, 0x2, 0x1b5, 0x1b7, 0x5, 0x34, 0x1b, 0x2, 0x1b6, 0x1b4, 
       0x3, 0x2, 0x2, 0x2, 0x1b7, 0x1ba, 0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b6, 
       0x3, 0x2, 0x2, 0x2, 0x1b8, 0x1b9, 0x3, 0x2, 0x2, 0x2, 0x1b9, 0x33, 
       0x3, 0x2, 0x2, 0x2, 0x1ba, 0x1b8, 0x3, 0x2, 0x2, 0x2, 0x1bb, 0x1bd, 
       0x5, 0x38, 0x1d, 0x2, 0x1bc, 0x1be, 0x5, 0x36, 0x1c, 0x2, 0x1bd, 
       0x1bc, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1be, 0x3, 0x2, 0x2, 0x2, 0x1be, 
       0x1bf, 0x3, 0x2, 0x2, 0x2, 0x1bf, 0x1c0, 0x5, 0x94, 0x4b, 0x2, 0x1c0, 
       0x35, 0x3, 0x2, 0x2, 0x2, 0x1c1, 0x1c2, 0x7, 0x2a, 0x2, 0x2, 0x1c2, 
       0x1c3, 0x5, 0x74, 0x3b, 0x2, 0x1c3, 0x37, 0x3, 0x2, 0x2, 0x2, 0x1c4, 
       0x1c6, 0x7, 0x5b, 0x2, 0x2, 0x1c5, 0x1c7, 0x5, 0x90, 0x49, 0x2, 0x1c6, 
       0x1c5, 0x3, 0x2, 0x2, 0x2, 0x1c6, 0x1c7, 0x3, 0x2, 0x2, 0x2, 0x1c7, 
       0x1c9, 0x3, 0x2, 0x2, 0x2, 0x1c8, 0x1ca, 0x5, 0x3a, 0x1e, 0x2, 0x1c9, 
       0x1c8, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1ca, 
       0x39, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x1ce, 0x5, 0x3c, 0x1f, 0x2, 0x1cc, 
       0x1cd, 0x7, 0x11, 0x2, 0x2, 0x1cd, 0x1cf, 0x5, 0x74, 0x3b, 0x2, 0x1ce, 
       0x1cc, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x1cf, 0x3, 0x2, 0x2, 0x2, 0x1cf, 
       0x1d5, 0x3, 0x2, 0x2, 0x2, 0x1d0, 0x1d1, 0x7, 0x11, 0x2, 0x2, 0x1d1, 
       0x1d5, 0x5, 0x74, 0x3b, 0x2, 0x1d2, 0x1d3, 0x7, 0x2b, 0x2, 0x2, 0x1d3, 
       0x1d5, 0x5, 0x74, 0x3b, 0x2, 0x1d4, 0x1cb, 0x3, 0x2, 0x2, 0x2, 0x1d4, 
       0x1d0, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d2, 0x3, 0x2, 0x2, 0x2, 0x1d5, 
       0x3b, 0x3, 0x2, 0x2, 0x2, 0x1d6, 0x1d8, 0x7, 0x13, 0x2, 0x2, 0x1d7, 
       0x1d9, 0x5, 0x3e, 0x20, 0x2, 0x1d8, 0x1d7, 0x3, 0x2, 0x2, 0x2, 0x1d8, 
       0x1d9, 0x3, 0x2, 0x2, 0x2, 0x1d9, 0x1da, 0x3, 0x2, 0x2, 0x2, 0x1da, 
       0x1db, 0x7, 0x15, 0x2, 0x2, 0x1db, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x1dc, 
       0x1e1, 0x5, 0x40, 0x21, 0x2, 0x1dd, 0x1de, 0x7, 0x17, 0x2, 0x2, 0x1de, 
       0x1e0, 0x5, 0x40, 0x21, 0x2, 0x1df, 0x1dd, 0x3, 0x2, 0x2, 0x2, 0x1e0, 
       0x1e3, 0x3, 0x2, 0x2, 0x2, 0x1e1, 0x1df, 0x3, 0x2, 0x2, 0x2, 0x1e1, 
       0x1e2, 0x3, 0x2, 0x2, 0x2, 0x1e2, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x1e3, 
       0x1e1, 0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1e7, 0x5, 0x42, 0x22, 0x2, 0x1e5, 
       0x1e7, 0x5, 0x46, 0x24, 0x2, 0x1e6, 0x1e4, 0x3, 0x2, 0x2, 0x2, 0x1e6, 
       0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1e7, 0x41, 0x3, 0x2, 0x2, 0x2, 0x1e8, 
       0x1ea, 0x7, 0x52, 0x2, 0x2, 0x1e9, 0x1e8, 0x3, 0x2, 0x2, 0x2, 0x1e9, 
       0x1ea, 0x3, 0x2, 0x2, 0x2, 0x1ea, 0x1ec, 0x3, 0x2, 0x2, 0x2, 0x1eb, 
       0x1ed, 0x7, 0x54, 0x2, 0x2, 0x1ec, 0x1eb, 0x3, 0x2, 0x2, 0x2, 0x1ec, 
       0x1ed, 0x3, 0x2, 0x2, 0x2, 0x1ed, 0x1f0, 0x3, 0x2, 0x2, 0x2, 0x1ee, 
       0x1f1, 0x5, 0x44, 0x23, 0x2, 0x1ef, 0x1f1, 0x5, 0x48, 0x25, 0x2, 
       0x1f0, 0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1f0, 0x1ef, 0x3, 0x2, 0x2, 0x2, 
       0x1f1, 0x43, 0x3, 0x2, 0x2, 0x2, 0x1f2, 0x1f4, 0x5, 0x80, 0x41, 0x2, 
       0x1f3, 0x1f5, 0x5, 0x3a, 0x1e, 0x2, 0x1f4, 0x1f3, 0x3, 0x2, 0x2, 
       0x2, 0x1f4, 0x1f5, 0x3, 0x2, 0x2, 0x2, 0x1f5, 0x1f6, 0x3, 0x2, 0x2, 
       0x2, 0x1f6, 0x1f7, 0x5, 0x96, 0x4c, 0x2, 0x1f7, 0x45, 0x3, 0x2, 0x2, 
       0x2, 0x1f8, 0x1fa, 0x7, 0x57, 0x2, 0x2, 0x1f9, 0x1fb, 0x7, 0x52, 
       0x2, 0x2, 0x1fa, 0x1f9, 0x3, 0x2, 0x2, 0x2, 0x1fa, 0x1fb, 0x3, 0x2, 
       0x2, 0x2, 0x1fb, 0x1fd, 0x3, 0x2, 0x2, 0x2, 0x1fc, 0x1fe, 0x7, 0x54, 
       0x2, 0x2, 0x1fd, 0x1fc, 0x3, 0x2, 0x2, 0x2, 0x1fd, 0x1fe, 0x3, 0x2, 
       0x2, 0x2, 0x1fe, 0x204, 0x3, 0x2, 0x2, 0x2, 0x1ff, 0x202, 0x5, 0x4e, 
       0x28, 0x2, 0x200, 0x202, 0x5, 0x4a, 0x26, 0x2, 0x201, 0x1ff, 0x3, 
       0x2, 0x2, 0x2, 0x201, 0x200, 0x3, 0x2, 0x2, 0x2, 0x202, 0x205, 0x3, 
       0x2, 0x2, 0x2, 0x203, 0x205, 0x5, 0x48, 0x25, 0x2, 0x204, 0x201, 
       0x3, 0x2, 0x2, 0x2, 0x204, 0x203, 0x3, 0x2, 0x2, 0x2, 0x205, 0x47, 
       0x3, 0x2, 0x2, 0x2, 0x206, 0x209, 0x7, 0x1c, 0x2, 0x2, 0x207, 0x20a, 
       0x5, 0x4e, 0x28, 0x2, 0x208, 0x20a, 0x5, 0x4a, 0x26, 0x2, 0x209, 
       0x207, 0x3, 0x2, 0x2, 0x2, 0x209, 0x208, 0x3, 0x2, 0x2, 0x2, 0x20a, 
       0x20c, 0x3, 0x2, 0x2, 0x2, 0x20b, 0x20d, 0x5, 0x28, 0x15, 0x2, 0x20c, 
       0x20b, 0x3, 0x2, 0x2, 0x2, 0x20c, 0x20d, 0x3, 0x2, 0x2, 0x2, 0x20d, 
       0x49, 0x3, 0x2, 0x2, 0x2, 0x20e, 0x20f, 0x5, 0x2c, 0x17, 0x2, 0x20f, 
       0x210, 0x5, 0x30, 0x19, 0x2, 0x210, 0x211, 0x5, 0x4c, 0x27, 0x2, 
       0x211, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x212, 0x213, 0x5, 0x38, 0x1d, 0x2, 
       0x213, 0x214, 0x5, 0x94, 0x4b, 0x2, 0x214, 0x4d, 0x3, 0x2, 0x2, 0x2, 
       0x215, 0x216, 0x5, 0x8, 0x5, 0x2, 0x216, 0x217, 0x7, 0x5b, 0x2, 0x2, 
       0x217, 0x22c, 0x7, 0x11, 0x2, 0x2, 0x218, 0x219, 0x5, 0xe, 0x8, 0x2, 
       0x219, 0x21b, 0x5, 0x80, 0x41, 0x2, 0x21a, 0x21c, 0x5, 0x90, 0x49, 
       0x2, 0x21b, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x21b, 0x21c, 0x3, 0x2, 0x2, 
       0x2, 0x21c, 0x21e, 0x3, 0x2, 0x2, 0x2, 0x21d, 0x21f, 0x5, 0x3c, 0x1f, 
       0x2, 0x21e, 0x21d, 0x3, 0x2, 0x2, 0x2, 0x21e, 0x21f, 0x3, 0x2, 0x2, 
       0x2, 0x21f, 0x220, 0x3, 0x2, 0x2, 0x2, 0x220, 0x221, 0x5, 0x94, 0x4b, 
       0x2, 0x221, 0x22d, 0x3, 0x2, 0x2, 0x2, 0x222, 0x223, 0x7, 0x12, 0x2, 
       0x2, 0x223, 0x228, 0x7, 0x13, 0x2, 0x2, 0x224, 0x226, 0x5, 0x10, 
       0x9, 0x2, 0x225, 0x224, 0x3, 0x2, 0x2, 0x2, 0x225, 0x226, 0x3, 0x2, 
       0x2, 0x2, 0x226, 0x229, 0x3, 0x2, 0x2, 0x2, 0x227, 0x229, 0x7, 0x14, 
       0x2, 0x2, 0x228, 0x225, 0x3, 0x2, 0x2, 0x2, 0x228, 0x227, 0x3, 0x2, 
       0x2, 0x2, 0x229, 0x22a, 0x3, 0x2, 0x2, 0x2, 0x22a, 0x22b, 0x7, 0x15, 
       0x2, 0x2, 0x22b, 0x22d, 0x5, 0x94, 0x4b, 0x2, 0x22c, 0x218, 0x3, 
       0x2, 0x2, 0x2, 0x22c, 0x222, 0x3, 0x2, 0x2, 0x2, 0x22d, 0x4f, 0x3, 
       0x2, 0x2, 0x2, 0x22e, 0x22f, 0x5, 0x5a, 0x2e, 0x2, 0x22f, 0x230, 
       0x7, 0x3, 0x2, 0x2, 0x230, 0x232, 0x3, 0x2, 0x2, 0x2, 0x231, 0x22e, 
       0x3, 0x2, 0x2, 0x2, 0x232, 0x235, 0x3, 0x2, 0x2, 0x2, 0x233, 0x231, 
       0x3, 0x2, 0x2, 0x2, 0x233, 0x234, 0x3, 0x2, 0x2, 0x2, 0x234, 0x51, 
       0x3, 0x2, 0x2, 0x2, 0x235, 0x233, 0x3, 0x2, 0x2, 0x2, 0x236, 0x238, 
       0x7, 0x5a, 0x2, 0x2, 0x237, 0x236, 0x3, 0x2, 0x2, 0x2, 0x237, 0x238, 
       0x3, 0x2, 0x2, 0x2, 0x238, 0x239, 0x3, 0x2, 0x2, 0x2, 0x239, 0x23a, 
       0x7, 0x2c, 0x2, 0x2, 0x23a, 0x23b, 0x5, 0x50, 0x29, 0x2, 0x23b, 0x53, 
       0x3, 0x2, 0x2, 0x2, 0x23c, 0x23d, 0x5, 0x5e, 0x30, 0x2, 0x23d, 0x23e, 
       0x7, 0x3, 0x2, 0x2, 0x23e, 0x240, 0x3, 0x2, 0x2, 0x2, 0x23f, 0x23c, 
       0x3, 0x2, 0x2, 0x2, 0x240, 0x243, 0x3, 0x2, 0x2, 0x2, 0x241, 0x23f, 
       0x3, 0x2, 0x2, 0x2, 0x241, 0x242, 0x3, 0x2, 0x2, 0x2, 0x242, 0x55, 
       0x3, 0x2, 0x2, 0x2, 0x243, 0x241, 0x3, 0x2, 0x2, 0x2, 0x244, 0x246, 
       0x7, 0x5a, 0x2, 0x2, 0x245, 0x244, 0x3, 0x2, 0x2, 0x2, 0x245, 0x246, 
       0x3, 0x2, 0x2, 0x2, 0x246, 0x247, 0x3, 0x2, 0x2, 0x2, 0x247, 0x248, 
       0x7, 0x2d, 0x2, 0x2, 0x248, 0x249, 0x5, 0x54, 0x2b, 0x2, 0x249, 0x57, 
       0x3, 0x2, 0x2, 0x2, 0x24a, 0x24b, 0x5, 0x76, 0x3c, 0x2, 0x24b, 0x24c, 
       0x7, 0x11, 0x2, 0x2, 0x24c, 0x24d, 0x5, 0x74, 0x3b, 0x2, 0x24d, 0x256, 
       0x3, 0x2, 0x2, 0x2, 0x24e, 0x256, 0x5, 0x60, 0x31, 0x2, 0x24f, 0x256, 
       0x5, 0x64, 0x33, 0x2, 0x250, 0x256, 0x5, 0x72, 0x3a, 0x2, 0x251, 
       0x256, 0x5, 0x6e, 0x38, 0x2, 0x252, 0x253, 0x5, 0x7c, 0x3f, 0x2, 
       0x253, 0x254, 0x5, 0x82, 0x42, 0x2, 0x254, 0x256, 0x3, 0x2, 0x2, 
       0x2, 0x255, 0x24a, 0x3, 0x2, 0x2, 0x2, 0x255, 0x24e, 0x3, 0x2, 0x2, 
       0x2, 0x255, 0x24f, 0x3, 0x2, 0x2, 0x2, 0x255, 0x250, 0x3, 0x2, 0x2, 
       0x2, 0x255, 0x251, 0x3, 0x2, 0x2, 0x2, 0x255, 0x252, 0x3, 0x2, 0x2, 
       0x2, 0x256, 0x59, 0x3, 0x2, 0x2, 0x2, 0x257, 0x258, 0x5, 0x58, 0x2d, 
       0x2, 0x258, 0x259, 0x5, 0x94, 0x4b, 0x2, 0x259, 0x5b, 0x3, 0x2, 0x2, 
       0x2, 0x25a, 0x25e, 0x5, 0x80, 0x41, 0x2, 0x25b, 0x25c, 0x7, 0x2b, 
       0x2, 0x2, 0x25c, 0x25f, 0x5, 0x74, 0x3b, 0x2, 0x25d, 0x25f, 0x5, 
       0x82, 0x42, 0x2, 0x25e, 0x25b, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x25d, 
       0x3, 0x2, 0x2, 0x2, 0x25f, 0x275, 0x3, 0x2, 0x2, 0x2, 0x260, 0x261, 
       0x7, 0x13, 0x2, 0x2, 0x261, 0x266, 0x5, 0x80, 0x41, 0x2, 0x262, 0x263, 
       0x7, 0x17, 0x2, 0x2, 0x263, 0x265, 0x5, 0x80, 0x41, 0x2, 0x264, 0x262, 
       0x3, 0x2, 0x2, 0x2, 0x265, 0x268, 0x3, 0x2, 0x2, 0x2, 0x266, 0x264, 
       0x3, 0x2, 0x2, 0x2, 0x266, 0x267, 0x3, 0x2, 0x2, 0x2, 0x267, 0x269, 
       0x3, 0x2, 0x2, 0x2, 0x268, 0x266, 0x3, 0x2, 0x2, 0x2, 0x269, 0x26a, 
       0x7, 0x15, 0x2, 0x2, 0x26a, 0x26b, 0x7, 0x2b, 0x2, 0x2, 0x26b, 0x26c, 
       0x5, 0x80, 0x41, 0x2, 0x26c, 0x26d, 0x5, 0x82, 0x42, 0x2, 0x26d, 
       0x275, 0x3, 0x2, 0x2, 0x2, 0x26e, 0x275, 0x7, 0x2e, 0x2, 0x2, 0x26f, 
       0x275, 0x7, 0x2f, 0x2, 0x2, 0x270, 0x275, 0x5, 0x62, 0x32, 0x2, 0x271, 
       0x275, 0x5, 0x66, 0x34, 0x2, 0x272, 0x275, 0x5, 0x6c, 0x37, 0x2, 
       0x273, 0x275, 0x5, 0x70, 0x39, 0x2, 0x274, 0x25a, 0x3, 0x2, 0x2, 
       0x2, 0x274, 0x260, 0x3, 0x2, 0x2, 0x2, 0x274, 0x26e, 0x3, 0x2, 0x2, 
       0x2, 0x274, 0x26f, 0x3, 0x2, 0x2, 0x2, 0x274, 0x270, 0x3, 0x2, 0x2, 
       0x2, 0x274, 0x271, 0x3, 0x2, 0x2, 0x2, 0x274, 0x272, 0x3, 0x2, 0x2, 
       0x2, 0x274, 0x273, 0x3, 0x2, 0x2, 0x2, 0x275, 0x5d, 0x3, 0x2, 0x2, 
       0x2, 0x276, 0x277, 0x5, 0x5c, 0x2f, 0x2, 0x277, 0x278, 0x5, 0x94, 
       0x4b, 0x2, 0x278, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x279, 0x27a, 0x7, 0x2a, 
       0x2, 0x2, 0x27a, 0x27b, 0x5, 0x74, 0x3b, 0x2, 0x27b, 0x27c, 0x7, 
       0x30, 0x2, 0x2, 0x27c, 0x284, 0x5, 0x50, 0x29, 0x2, 0x27d, 0x27e, 
       0x7, 0x31, 0x2, 0x2, 0x27e, 0x27f, 0x5, 0x74, 0x3b, 0x2, 0x27f, 0x280, 
       0x7, 0x30, 0x2, 0x2, 0x280, 0x281, 0x5, 0x50, 0x29, 0x2, 0x281, 0x283, 
       0x3, 0x2, 0x2, 0x2, 0x282, 0x27d, 0x3, 0x2, 0x2, 0x2, 0x283, 0x286, 
       0x3, 0x2, 0x2, 0x2, 0x284, 0x282, 0x3, 0x2, 0x2, 0x2, 0x284, 0x285, 
       0x3, 0x2, 0x2, 0x2, 0x285, 0x289, 0x3, 0x2, 0x2, 0x2, 0x286, 0x284, 
       0x3, 0x2, 0x2, 0x2, 0x287, 0x288, 0x7, 0x32, 0x2, 0x2, 0x288, 0x28a, 
       0x5, 0x50, 0x29, 0x2, 0x289, 0x287, 0x3, 0x2, 0x2, 0x2, 0x289, 0x28a, 
       0x3, 0x2, 0x2, 0x2, 0x28a, 0x28b, 0x3, 0x2, 0x2, 0x2, 0x28b, 0x28c, 
       0x7, 0x10, 0x2, 0x2, 0x28c, 0x28d, 0x7, 0x2a, 0x2, 0x2, 0x28d, 0x61, 
       0x3, 0x2, 0x2, 0x2, 0x28e, 0x28f, 0x7, 0x2a, 0x2, 0x2, 0x28f, 0x290, 
       0x5, 0x74, 0x3b, 0x2, 0x290, 0x291, 0x7, 0x30, 0x2, 0x2, 0x291, 0x299, 
       0x5, 0x54, 0x2b, 0x2, 0x292, 0x293, 0x7, 0x31, 0x2, 0x2, 0x293, 0x294, 
       0x5, 0x74, 0x3b, 0x2, 0x294, 0x295, 0x7, 0x30, 0x2, 0x2, 0x295, 0x296, 
       0x5, 0x54, 0x2b, 0x2, 0x296, 0x298, 0x3, 0x2, 0x2, 0x2, 0x297, 0x292, 
       0x3, 0x2, 0x2, 0x2, 0x298, 0x29b, 0x3, 0x2, 0x2, 0x2, 0x299, 0x297, 
       0x3, 0x2, 0x2, 0x2, 0x299, 0x29a, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x29e, 
       0x3, 0x2, 0x2, 0x2, 0x29b, 0x299, 0x3, 0x2, 0x2, 0x2, 0x29c, 0x29d, 
       0x7, 0x32, 0x2, 0x2, 0x29d, 0x29f, 0x5, 0x54, 0x2b, 0x2, 0x29e, 0x29c, 
       0x3, 0x2, 0x2, 0x2, 0x29e, 0x29f, 0x3, 0x2, 0x2, 0x2, 0x29f, 0x2a0, 
       0x3, 0x2, 0x2, 0x2, 0x2a0, 0x2a1, 0x7, 0x10, 0x2, 0x2, 0x2a1, 0x2a2, 
       0x7, 0x2a, 0x2, 0x2, 0x2a2, 0x63, 0x3, 0x2, 0x2, 0x2, 0x2a3, 0x2a4, 
       0x7, 0x33, 0x2, 0x2, 0x2a4, 0x2a5, 0x5, 0x68, 0x35, 0x2, 0x2a5, 0x2a6, 
       0x7, 0x34, 0x2, 0x2, 0x2a6, 0x2a7, 0x5, 0x50, 0x29, 0x2, 0x2a7, 0x2a8, 
       0x7, 0x10, 0x2, 0x2, 0x2a8, 0x2a9, 0x7, 0x33, 0x2, 0x2, 0x2a9, 0x65, 
       0x3, 0x2, 0x2, 0x2, 0x2aa, 0x2ab, 0x7, 0x33, 0x2, 0x2, 0x2ab, 0x2ac, 
       0x5, 0x68, 0x35, 0x2, 0x2ac, 0x2ad, 0x7, 0x34, 0x2, 0x2, 0x2ad, 0x2ae, 
       0x5, 0x54, 0x2b, 0x2, 0x2ae, 0x2af, 0x7, 0x10, 0x2, 0x2, 0x2af, 0x2b0, 
       0x7, 0x33, 0x2, 0x2, 0x2b0, 0x67, 0x3, 0x2, 0x2, 0x2, 0x2b1, 0x2b6, 
       0x5, 0x6a, 0x36, 0x2, 0x2b2, 0x2b3, 0x7, 0x17, 0x2, 0x2, 0x2b3, 0x2b5, 
       0x5, 0x6a, 0x36, 0x2, 0x2b4, 0x2b2, 0x3, 0x2, 0x2, 0x2, 0x2b5, 0x2b8, 
       0x3, 0x2, 0x2, 0x2, 0x2b6, 0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2b6, 0x2b7, 
       0x3, 0x2, 0x2, 0x2, 0x2b7, 0x69, 0x3, 0x2, 0x2, 0x2, 0x2b8, 0x2b6, 
       0x3, 0x2, 0x2, 0x2, 0x2b9, 0x2bc, 0x7, 0x5b, 0x2, 0x2, 0x2ba, 0x2bb, 
       0x7, 0x35, 0x2, 0x2, 0x2bb, 0x2bd, 0x5, 0x74, 0x3b, 0x2, 0x2bc, 0x2ba, 
       0x3, 0x2, 0x2, 0x2, 0x2bc, 0x2bd, 0x3, 0x2, 0x2, 0x2, 0x2bd, 0x6b, 
       0x3, 0x2, 0x2, 0x2, 0x2be, 0x2bf, 0x7, 0x36, 0x2, 0x2, 0x2bf, 0x2c0, 
       0x5, 0x74, 0x3b, 0x2, 0x2c0, 0x2c1, 0x7, 0x34, 0x2, 0x2, 0x2c1, 0x2c2, 
       0x5, 0x54, 0x2b, 0x2, 0x2c2, 0x2c3, 0x7, 0x10, 0x2, 0x2, 0x2c3, 0x2c4, 
       0x7, 0x36, 0x2, 0x2, 0x2c4, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x2c5, 0x2c6, 
       0x7, 0x37, 0x2, 0x2, 0x2c6, 0x2c7, 0x5, 0x74, 0x3b, 0x2, 0x2c7, 0x2c8, 
       0x7, 0x30, 0x2, 0x2, 0x2c8, 0x2d0, 0x5, 0x50, 0x29, 0x2, 0x2c9, 0x2ca, 
       0x7, 0x38, 0x2, 0x2, 0x2ca, 0x2cb, 0x5, 0x74, 0x3b, 0x2, 0x2cb, 0x2cc, 
       0x7, 0x30, 0x2, 0x2, 0x2cc, 0x2cd, 0x5, 0x50, 0x29, 0x2, 0x2cd, 0x2cf, 
       0x3, 0x2, 0x2, 0x2, 0x2ce, 0x2c9, 0x3, 0x2, 0x2, 0x2, 0x2cf, 0x2d2, 
       0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2ce, 0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2d1, 
       0x3, 0x2, 0x2, 0x2, 0x2d1, 0x2d3, 0x3, 0x2, 0x2, 0x2, 0x2d2, 0x2d0, 
       0x3, 0x2, 0x2, 0x2, 0x2d3, 0x2d4, 0x7, 0x10, 0x2, 0x2, 0x2d4, 0x2d5, 
       0x7, 0x37, 0x2, 0x2, 0x2d5, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x2d6, 0x2d7, 
       0x7, 0x37, 0x2, 0x2, 0x2d7, 0x2d8, 0x5, 0x74, 0x3b, 0x2, 0x2d8, 0x2d9, 
       0x7, 0x30, 0x2, 0x2, 0x2d9, 0x2e1, 0x5, 0x54, 0x2b, 0x2, 0x2da, 0x2db, 
       0x7, 0x38, 0x2, 0x2, 0x2db, 0x2dc, 0x5, 0x74, 0x3b, 0x2, 0x2dc, 0x2dd, 
       0x7, 0x30, 0x2, 0x2, 0x2dd, 0x2de, 0x5, 0x54, 0x2b, 0x2, 0x2de, 0x2e0, 
       0x3, 0x2, 0x2, 0x2, 0x2df, 0x2da, 0x3, 0x2, 0x2, 0x2, 0x2e0, 0x2e3, 
       0x3, 0x2, 0x2, 0x2, 0x2e1, 0x2df, 0x3, 0x2, 0x2, 0x2, 0x2e1, 0x2e2, 
       0x3, 0x2, 0x2, 0x2, 0x2e2, 0x2e4, 0x3, 0x2, 0x2, 0x2, 0x2e3, 0x2e1, 
       0x3, 0x2, 0x2, 0x2, 0x2e4, 0x2e5, 0x7, 0x10, 0x2, 0x2, 0x2e5, 0x2e6, 
       0x7, 0x37, 0x2, 0x2, 0x2e6, 0x71, 0x3, 0x2, 0x2, 0x2, 0x2e7, 0x2e8, 
       0x7, 0x39, 0x2, 0x2, 0x2e8, 0x2e9, 0x7, 0x13, 0x2, 0x2, 0x2e9, 0x2ea, 
       0x5, 0x80, 0x41, 0x2, 0x2ea, 0x2eb, 0x7, 0x17, 0x2, 0x2, 0x2eb, 0x2ec, 
       0x5, 0x80, 0x41, 0x2, 0x2ec, 0x2ed, 0x7, 0x15, 0x2, 0x2, 0x2ed, 0x73, 
       0x3, 0x2, 0x2, 0x2, 0x2ee, 0x301, 0x5, 0x76, 0x3c, 0x2, 0x2ef, 0x2f0, 
       0x7, 0x2a, 0x2, 0x2, 0x2f0, 0x2f1, 0x5, 0x74, 0x3b, 0x2, 0x2f1, 0x2f2, 
       0x7, 0x30, 0x2, 0x2, 0x2f2, 0x2fa, 0x5, 0x74, 0x3b, 0x2, 0x2f3, 0x2f4, 
       0x7, 0x31, 0x2, 0x2, 0x2f4, 0x2f5, 0x5, 0x74, 0x3b, 0x2, 0x2f5, 0x2f6, 
       0x7, 0x30, 0x2, 0x2, 0x2f6, 0x2f7, 0x5, 0x74, 0x3b, 0x2, 0x2f7, 0x2f9, 
       0x3, 0x2, 0x2, 0x2, 0x2f8, 0x2f3, 0x3, 0x2, 0x2, 0x2, 0x2f9, 0x2fc, 
       0x3, 0x2, 0x2, 0x2, 0x2fa, 0x2f8, 0x3, 0x2, 0x2, 0x2, 0x2fa, 0x2fb, 
       0x3, 0x2, 0x2, 0x2, 0x2fb, 0x2fd, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2fa, 
       0x3, 0x2, 0x2, 0x2, 0x2fd, 0x2fe, 0x7, 0x32, 0x2, 0x2, 0x2fe, 0x2ff, 
       0x5, 0x74, 0x3b, 0x2, 0x2ff, 0x301, 0x3, 0x2, 0x2, 0x2, 0x300, 0x2ee, 
       0x3, 0x2, 0x2, 0x2, 0x300, 0x2ef, 0x3, 0x2, 0x2, 0x2, 0x301, 0x75, 
       0x3, 0x2, 0x2, 0x2, 0x302, 0x309, 0x5, 0x78, 0x3d, 0x2, 0x303, 0x304, 
       0x7, 0x14, 0x2, 0x2, 0x304, 0x307, 0x5, 0x78, 0x3d, 0x2, 0x305, 0x306, 
       0x7, 0x14, 0x2, 0x2, 0x306, 0x308, 0x5, 0x78, 0x3d, 0x2, 0x307, 0x305, 
       0x3, 0x2, 0x2, 0x2, 0x307, 0x308, 0x3, 0x2, 0x2, 0x2, 0x308, 0x30a, 
       0x3, 0x2, 0x2, 0x2, 0x309, 0x303, 0x3, 0x2, 0x2, 0x2, 0x309, 0x30a, 
       0x3, 0x2, 0x2, 0x2, 0x30a, 0x77, 0x3, 0x2, 0x2, 0x2, 0x30b, 0x30c, 
       0x8, 0x3d, 0x1, 0x2, 0x30c, 0x30d, 0x9, 0x6, 0x2, 0x2, 0x30d, 0x316, 
       0x5, 0x78, 0x3d, 0xb, 0x30e, 0x30f, 0x5, 0x7a, 0x3e, 0x2, 0x30f, 
       0x310, 0x9, 0x7, 0x2, 0x2, 0x310, 0x311, 0x5, 0x7a, 0x3e, 0x2, 0x311, 
       0x316, 0x3, 0x2, 0x2, 0x2, 0x312, 0x313, 0x7, 0x49, 0x2, 0x2, 0x313, 
       0x316, 0x5, 0x78, 0x3d, 0x6, 0x314, 0x316, 0x5, 0x7a, 0x3e, 0x2, 
       0x315, 0x30b, 0x3, 0x2, 0x2, 0x2, 0x315, 0x30e, 0x3, 0x2, 0x2, 0x2, 
       0x315, 0x312, 0x3, 0x2, 0x2, 0x2, 0x315, 0x314, 0x3, 0x2, 0x2, 0x2, 
       0x316, 0x328, 0x3, 0x2, 0x2, 0x2, 0x317, 0x318, 0xc, 0x9, 0x2, 0x2, 
       0x318, 0x319, 0x9, 0x8, 0x2, 0x2, 0x319, 0x327, 0x5, 0x78, 0x3d, 
       0xa, 0x31a, 0x31b, 0xc, 0x8, 0x2, 0x2, 0x31b, 0x31c, 0x9, 0x9, 0x2, 
       0x2, 0x31c, 0x327, 0x5, 0x78, 0x3d, 0x9, 0x31d, 0x31e, 0xc, 0x7, 
       0x2, 0x2, 0x31e, 0x31f, 0x9, 0xa, 0x2, 0x2, 0x31f, 0x327, 0x5, 0x78, 
       0x3d, 0x8, 0x320, 0x321, 0xc, 0x5, 0x2, 0x2, 0x321, 0x322, 0x7, 0x4a, 
       0x2, 0x2, 0x322, 0x327, 0x5, 0x78, 0x3d, 0x6, 0x323, 0x324, 0xc, 
       0x4, 0x2, 0x2, 0x324, 0x325, 0x7, 0x4b, 0x2, 0x2, 0x325, 0x327, 0x5, 
       0x78, 0x3d, 0x5, 0x326, 0x317, 0x3, 0x2, 0x2, 0x2, 0x326, 0x31a, 
       0x3, 0x2, 0x2, 0x2, 0x326, 0x31d, 0x3, 0x2, 0x2, 0x2, 0x326, 0x320, 
       0x3, 0x2, 0x2, 0x2, 0x326, 0x323, 0x3, 0x2, 0x2, 0x2, 0x327, 0x32a, 
       0x3, 0x2, 0x2, 0x2, 0x328, 0x326, 0x3, 0x2, 0x2, 0x2, 0x328, 0x329, 
       0x3, 0x2, 0x2, 0x2, 0x329, 0x79, 0x3, 0x2, 0x2, 0x2, 0x32a, 0x328, 
       0x3, 0x2, 0x2, 0x2, 0x32b, 0x34c, 0x7, 0x5d, 0x2, 0x2, 0x32c, 0x34c, 
       0x7, 0x5c, 0x2, 0x2, 0x32d, 0x34c, 0x7, 0x4c, 0x2, 0x2, 0x32e, 0x34c, 
       0x7, 0x4d, 0x2, 0x2, 0x32f, 0x330, 0x5, 0x80, 0x41, 0x2, 0x330, 0x331, 
       0x5, 0x82, 0x42, 0x2, 0x331, 0x34c, 0x3, 0x2, 0x2, 0x2, 0x332, 0x333, 
       0x7, 0x16, 0x2, 0x2, 0x333, 0x34c, 0x5, 0x82, 0x42, 0x2, 0x334, 0x335, 
       0x7, 0x5a, 0x2, 0x2, 0x335, 0x34c, 0x5, 0x82, 0x42, 0x2, 0x336, 0x34c, 
       0x5, 0x80, 0x41, 0x2, 0x337, 0x338, 0x7, 0x13, 0x2, 0x2, 0x338, 0x339, 
       0x5, 0x8c, 0x47, 0x2, 0x339, 0x33a, 0x7, 0x15, 0x2, 0x2, 0x33a, 0x34c, 
       0x3, 0x2, 0x2, 0x2, 0x33b, 0x33c, 0x7, 0x4e, 0x2, 0x2, 0x33c, 0x341, 
       0x5, 0x8e, 0x48, 0x2, 0x33d, 0x33e, 0x7, 0x3, 0x2, 0x2, 0x33e, 0x340, 
       0x5, 0x8e, 0x48, 0x2, 0x33f, 0x33d, 0x3, 0x2, 0x2, 0x2, 0x340, 0x343, 
       0x3, 0x2, 0x2, 0x2, 0x341, 0x33f, 0x3, 0x2, 0x2, 0x2, 0x341, 0x342, 
       0x3, 0x2, 0x2, 0x2, 0x342, 0x344, 0x3, 0x2, 0x2, 0x2, 0x343, 0x341, 
       0x3, 0x2, 0x2, 0x2, 0x344, 0x345, 0x7, 0x4f, 0x2, 0x2, 0x345, 0x34c, 
       0x3, 0x2, 0x2, 0x2, 0x346, 0x347, 0x7, 0x50, 0x2, 0x2, 0x347, 0x348, 
       0x5, 0x84, 0x43, 0x2, 0x348, 0x349, 0x7, 0x20, 0x2, 0x2, 0x349, 0x34c, 
       0x3, 0x2, 0x2, 0x2, 0x34a, 0x34c, 0x7, 0x10, 0x2, 0x2, 0x34b, 0x32b, 
       0x3, 0x2, 0x2, 0x2, 0x34b, 0x32c, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x32d, 
       0x3, 0x2, 0x2, 0x2, 0x34b, 0x32e, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x32f, 
       0x3, 0x2, 0x2, 0x2, 0x34b, 0x332, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x334, 
       0x3, 0x2, 0x2, 0x2, 0x34b, 0x336, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x337, 
       0x3, 0x2, 0x2, 0x2, 0x34b, 0x33b, 0x3, 0x2, 0x2, 0x2, 0x34b, 0x346, 
       0x3, 0x2, 0x2, 0x2, 0x34b, 0x34a, 0x3, 0x2, 0x2, 0x2, 0x34c, 0x7b, 
       0x3, 0x2, 0x2, 0x2, 0x34d, 0x34f, 0x7, 0x29, 0x2, 0x2, 0x34e, 0x34d, 
       0x3, 0x2, 0x2, 0x2, 0x34e, 0x34f, 0x3, 0x2, 0x2, 0x2, 0x34f, 0x350, 
       0x3, 0x2, 0x2, 0x2, 0x350, 0x355, 0x7, 0x5b, 0x2, 0x2, 0x351, 0x352, 
       0x7, 0x29, 0x2, 0x2, 0x352, 0x354, 0x7, 0x5b, 0x2, 0x2, 0x353, 0x351, 
       0x3, 0x2, 0x2, 0x2, 0x354, 0x357, 0x3, 0x2, 0x2, 0x2, 0x355, 0x353, 
       0x3, 0x2, 0x2, 0x2, 0x355, 0x356, 0x3, 0x2, 0x2, 0x2, 0x356, 0x7d, 
       0x3, 0x2, 0x2, 0x2, 0x357, 0x355, 0x3, 0x2, 0x2, 0x2, 0x358, 0x35a, 
       0x7, 0x5b, 0x2, 0x2, 0x359, 0x35b, 0x5, 0x90, 0x49, 0x2, 0x35a, 0x359, 
       0x3, 0x2, 0x2, 0x2, 0x35a, 0x35b, 0x3, 0x2, 0x2, 0x2, 0x35b, 0x7f, 
       0x3, 0x2, 0x2, 0x2, 0x35c, 0x361, 0x5, 0x7e, 0x40, 0x2, 0x35d, 0x35e, 
       0x7, 0x29, 0x2, 0x2, 0x35e, 0x360, 0x5, 0x7e, 0x40, 0x2, 0x35f, 0x35d, 
       0x3, 0x2, 0x2, 0x2, 0x360, 0x363, 0x3, 0x2, 0x2, 0x2, 0x361, 0x35f, 
       0x3, 0x2, 0x2, 0x2, 0x361, 0x362, 0x3, 0x2, 0x2, 0x2, 0x362, 0x81, 
       0x3, 0x2, 0x2, 0x2, 0x363, 0x361, 0x3, 0x2, 0x2, 0x2, 0x364, 0x366, 
       0x7, 0x13, 0x2, 0x2, 0x365, 0x367, 0x5, 0x84, 0x43, 0x2, 0x366, 0x365, 
       0x3, 0x2, 0x2, 0x2, 0x366, 0x367, 0x3, 0x2, 0x2, 0x2, 0x367, 0x368, 
       0x3, 0x2, 0x2, 0x2, 0x368, 0x369, 0x7, 0x15, 0x2, 0x2, 0x369, 0x83, 
       0x3, 0x2, 0x2, 0x2, 0x36a, 0x371, 0x5, 0x8a, 0x46, 0x2, 0x36b, 0x36c, 
       0x7, 0x17, 0x2, 0x2, 0x36c, 0x370, 0x5, 0x8a, 0x46, 0x2, 0x36d, 0x36e, 
       0x7, 0x33, 0x2, 0x2, 0x36e, 0x370, 0x5, 0x68, 0x35, 0x2, 0x36f, 0x36b, 
       0x3, 0x2, 0x2, 0x2, 0x36f, 0x36d, 0x3, 0x2, 0x2, 0x2, 0x370, 0x373, 
       0x3, 0x2, 0x2, 0x2, 0x371, 0x36f, 0x3, 0x2, 0x2, 0x2, 0x371, 0x372, 
       0x3, 0x2, 0x2, 0x2, 0x372, 0x376, 0x3, 0x2, 0x2, 0x2, 0x373, 0x371, 
       0x3, 0x2, 0x2, 0x2, 0x374, 0x376, 0x5, 0x86, 0x44, 0x2, 0x375, 0x36a, 
       0x3, 0x2, 0x2, 0x2, 0x375, 0x374, 0x3, 0x2, 0x2, 0x2, 0x376, 0x85, 
       0x3, 0x2, 0x2, 0x2, 0x377, 0x37c, 0x5, 0x88, 0x45, 0x2, 0x378, 0x379, 
       0x7, 0x17, 0x2, 0x2, 0x379, 0x37b, 0x5, 0x88, 0x45, 0x2, 0x37a, 0x378, 
       0x3, 0x2, 0x2, 0x2, 0x37b, 0x37e, 0x3, 0x2, 0x2, 0x2, 0x37c, 0x37a, 
       0x3, 0x2, 0x2, 0x2, 0x37c, 0x37d, 0x3, 0x2, 0x2, 0x2, 0x37d, 0x87, 
       0x3, 0x2, 0x2, 0x2, 0x37e, 0x37c, 0x3, 0x2, 0x2, 0x2, 0x37f, 0x380, 
       0x7, 0x5b, 0x2, 0x2, 0x380, 0x381, 0x7, 0x11, 0x2, 0x2, 0x381, 0x382, 
       0x5, 0x8a, 0x46, 0x2, 0x382, 0x89, 0x3, 0x2, 0x2, 0x2, 0x383, 0x384, 
       0x7, 0xf, 0x2, 0x2, 0x384, 0x385, 0x5, 0x7c, 0x3f, 0x2, 0x385, 0x387, 
       0x7, 0x13, 0x2, 0x2, 0x386, 0x388, 0x5, 0x86, 0x44, 0x2, 0x387, 0x386, 
       0x3, 0x2, 0x2, 0x2, 0x387, 0x388, 0x3, 0x2, 0x2, 0x2, 0x388, 0x389, 
       0x3, 0x2, 0x2, 0x2, 0x389, 0x38a, 0x7, 0x15, 0x2, 0x2, 0x38a, 0x38d, 
       0x3, 0x2, 0x2, 0x2, 0x38b, 0x38d, 0x5, 0x74, 0x3b, 0x2, 0x38c, 0x383, 
       0x3, 0x2, 0x2, 0x2, 0x38c, 0x38b, 0x3, 0x2, 0x2, 0x2, 0x38d, 0x8b, 
       0x3, 0x2, 0x2, 0x2, 0x38e, 0x390, 0x5, 0x74, 0x3b, 0x2, 0x38f, 0x38e, 
       0x3, 0x2, 0x2, 0x2, 0x38f, 0x390, 0x3, 0x2, 0x2, 0x2, 0x390, 0x395, 
       0x3, 0x2, 0x2, 0x2, 0x391, 0x392, 0x7, 0x17, 0x2, 0x2, 0x392, 0x394, 
       0x5, 0x74, 0x3b, 0x2, 0x393, 0x391, 0x3, 0x2, 0x2, 0x2, 0x394, 0x397, 
       0x3, 0x2, 0x2, 0x2, 0x395, 0x393, 0x3, 0x2, 0x2, 0x2, 0x395, 0x396, 
       0x3, 0x2, 0x2, 0x2, 0x396, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x397, 0x395, 
       0x3, 0x2, 0x2, 0x2, 0x398, 0x39d, 0x5, 0x74, 0x3b, 0x2, 0x399, 0x39a, 
       0x7, 0x17, 0x2, 0x2, 0x39a, 0x39c, 0x5, 0x74, 0x3b, 0x2, 0x39b, 0x399, 
       0x3, 0x2, 0x2, 0x2, 0x39c, 0x39f, 0x3, 0x2, 0x2, 0x2, 0x39d, 0x39b, 
       0x3, 0x2, 0x2, 0x2, 0x39d, 0x39e, 0x3, 0x2, 0x2, 0x2, 0x39e, 0x8f, 
       0x3, 0x2, 0x2, 0x2, 0x39f, 0x39d, 0x3, 0x2, 0x2, 0x2, 0x3a0, 0x3a1, 
       0x7, 0x4e, 0x2, 0x2, 0x3a1, 0x3a6, 0x5, 0x92, 0x4a, 0x2, 0x3a2, 0x3a3, 
       0x7, 0x17, 0x2, 0x2, 0x3a3, 0x3a5, 0x5, 0x92, 0x4a, 0x2, 0x3a4, 0x3a2, 
       0x3, 0x2, 0x2, 0x2, 0x3a5, 0x3a8, 0x3, 0x2, 0x2, 0x2, 0x3a6, 0x3a4, 
       0x3, 0x2, 0x2, 0x2, 0x3a6, 0x3a7, 0x3, 0x2, 0x2, 0x2, 0x3a7, 0x3a9, 
       0x3, 0x2, 0x2, 0x2, 0x3a8, 0x3a6, 0x3, 0x2, 0x2, 0x2, 0x3a9, 0x3aa, 
       0x7, 0x4f, 0x2, 0x2, 0x3aa, 0x91, 0x3, 0x2, 0x2, 0x2, 0x3ab, 0x3ae, 
       0x7, 0x14, 0x2, 0x2, 0x3ac, 0x3ae, 0x5, 0x74, 0x3b, 0x2, 0x3ad, 0x3ab, 
       0x3, 0x2, 0x2, 0x2, 0x3ad, 0x3ac, 0x3, 0x2, 0x2, 0x2, 0x3ae, 0x93, 
       0x3, 0x2, 0x2, 0x2, 0x3af, 0x3b1, 0x5, 0x96, 0x4c, 0x2, 0x3b0, 0x3b2, 
       0x5, 0x98, 0x4d, 0x2, 0x3b1, 0x3b0, 0x3, 0x2, 0x2, 0x2, 0x3b1, 0x3b2, 
       0x3, 0x2, 0x2, 0x2, 0x3b2, 0x95, 0x3, 0x2, 0x2, 0x2, 0x3b3, 0x3b8, 
       0x7, 0x5c, 0x2, 0x2, 0x3b4, 0x3b5, 0x7, 0x3a, 0x2, 0x2, 0x3b5, 0x3b7, 
       0x7, 0x5c, 0x2, 0x2, 0x3b6, 0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3b7, 0x3ba, 
       0x3, 0x2, 0x2, 0x2, 0x3b8, 0x3b6, 0x3, 0x2, 0x2, 0x2, 0x3b8, 0x3b9, 
       0x3, 0x2, 0x2, 0x2, 0x3b9, 0x3bc, 0x3, 0x2, 0x2, 0x2, 0x3ba, 0x3b8, 
       0x3, 0x2, 0x2, 0x2, 0x3bb, 0x3b3, 0x3, 0x2, 0x2, 0x2, 0x3bb, 0x3bc, 
       0x3, 0x2, 0x2, 0x2, 0x3bc, 0x97, 0x3, 0x2, 0x2, 0x2, 0x3bd, 0x3be, 
       0x7, 0x51, 0x2, 0x2, 0x3be, 0x3bf, 0x5, 0x3c, 0x1f, 0x2, 0x3bf, 0x99, 
       0x3, 0x2, 0x2, 0x2, 0x79, 0x9c, 0x9f, 0xa4, 0xa8, 0xae, 0xb4, 0xbb, 
       0xc0, 0xc6, 0xc9, 0xcd, 0xda, 0xe3, 0xe6, 0xf5, 0xfe, 0x105, 0x10e, 
       0x11b, 0x11d, 0x122, 0x125, 0x128, 0x12b, 0x130, 0x137, 0x13c, 0x145, 
       0x14c, 0x14f, 0x152, 0x155, 0x158, 0x15c, 0x15f, 0x162, 0x165, 0x168, 
       0x16d, 0x172, 0x17e, 0x180, 0x189, 0x18f, 0x192, 0x197, 0x19c, 0x1a1, 
       0x1a4, 0x1a7, 0x1b0, 0x1b8, 0x1bd, 0x1c6, 0x1c9, 0x1ce, 0x1d4, 0x1d8, 
       0x1e1, 0x1e6, 0x1e9, 0x1ec, 0x1f0, 0x1f4, 0x1fa, 0x1fd, 0x201, 0x204, 
       0x209, 0x20c, 0x21b, 0x21e, 0x225, 0x228, 0x22c, 0x233, 0x237, 0x241, 
       0x245, 0x255, 0x25e, 0x266, 0x274, 0x284, 0x289, 0x299, 0x29e, 0x2b6, 
       0x2bc, 0x2d0, 0x2e1, 0x2fa, 0x300, 0x307, 0x309, 0x315, 0x326, 0x328, 
       0x341, 0x34b, 0x34e, 0x355, 0x35a, 0x361, 0x366, 0x36f, 0x371, 0x375, 
       0x37c, 0x387, 0x38c, 0x38f, 0x395, 0x39d, 0x3a6, 0x3ad, 0x3b1, 0x3b8, 
       0x3bb, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

ModelicaParser::Initializer ModelicaParser::_init;
