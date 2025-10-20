#pragma once

#include "lexer.h"
#include "token.h"
#include <algorithm>
#include <exception>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

enum class NodeKind {
	UnaryOp,
	BinaryOp,
	Literal,
	Identifier,
	FunctionCall,
	ArrayAccess,
	MemberAccess,
	Conditional,
	Cast,
	SizeOf,
	Assignment,
	Compound,

	LabelStmt,
	CompoundStmt,
	IfStmt,
	WhileStmt,
	DoWhileStmt, // TODO: Try eliding this
	ForStmt,
    GotoStmt,
    ContinueStmt,
    BreakStmt,
    ReturnStmt,

	VarDecl,
	StructDecl,
	EnumDecl,
	FunctionDecl,

	TypeSpec,
};


struct ASTNode{
	NodeKind kind;
	size_t line;

	ASTNode(NodeKind kind, size_t line) : kind(kind), line(line) {}
	virtual ~ASTNode() = default;
};

struct Expr : ASTNode {
	using ASTNode::ASTNode;
};

struct UnaryExpr : Expr {
	Type op;
	std::unique_ptr<Expr> operand;
	bool isPrefix;
	UnaryExpr(Type op, std::unique_ptr<Expr> operand, bool isPrefix, size_t line) : Expr(NodeKind::UnaryOp,line), op(op), operand(std::move(operand)), isPrefix(isPrefix) {}
};

struct BinaryExpr : Expr {
	Type op;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;

	BinaryExpr(Type op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, size_t line) : Expr(NodeKind::BinaryOp, line), op(op), left(std::move(left)), right(std::move(right)) {}
};

struct LiteralExpr : Expr {
	Token token;
	LiteralExpr(Token token) : Expr(NodeKind::Literal, token.line), token(std::move(token)) {}
};

struct IdentifierExpr : Expr {
	std::string_view name;

	IdentifierExpr(std::string_view name, size_t line) : Expr(NodeKind::Identifier,line), name(std::move(name)) {}
};

struct CallExpr : Expr {
	std::unique_ptr<Expr> callee;
	std::vector<std::unique_ptr<Expr>> args;

	CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> args, size_t line) : Expr(NodeKind::FunctionCall, line), callee(std::move(callee)), args(std::move(args)) {}
};

struct ArrayAccessExpr : Expr {
	std::unique_ptr<Expr> array;
	std::unique_ptr<Expr> index;

	ArrayAccessExpr(std::unique_ptr<Expr> array, std::unique_ptr<Expr> index, size_t line) : Expr(NodeKind::ArrayAccess, line), array(std::move(array)), index(std::move(index)) {}
};

struct MemberExpr : Expr {
	std::unique_ptr<Expr> object;
	std::string_view member;
	bool isArrow;

	MemberExpr(std::unique_ptr<Expr> object, std::string_view member, bool isArrow, size_t line) : Expr(NodeKind::MemberAccess,line), object(std::move(object)), member(member), isArrow(isArrow) {}
};

struct ConditionalExpr : Expr {
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Expr> thenExpr;
	std::unique_ptr<Expr> elseExpr;

	ConditionalExpr(std::unique_ptr<Expr> cond, std::unique_ptr<Expr> then, std::unique_ptr<Expr> elseExpr, size_t line) : Expr(NodeKind::Conditional,line), condition(std::move(cond)), thenExpr(std::move(then)), elseExpr(std::move(elseExpr)) {}
};

struct TypeSpec : ASTNode {
	std::vector<Type> qual;
	Type baseType;
	std::string_view structName;
	TypeSpec(Type base, size_t line) : ASTNode(NodeKind::TypeSpec, line), baseType(base) {}
};

struct Stmt : ASTNode {
	using ASTNode::ASTNode;
};

struct LabelStmt : Stmt {
    std::string_view label;
    std::unique_ptr<Stmt> statement;

    LabelStmt(std::string_view label, std::unique_ptr<Stmt> stmt, size_t line)
        : Stmt(NodeKind::LabelStmt, line), label(label), statement(std::move(stmt)) {}
};

struct CompoundStmt : Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;

    CompoundStmt(std::vector<std::unique_ptr<Stmt>> stmts, size_t line)
        : Stmt(NodeKind::CompoundStmt, line), statements(std::move(stmts)) {}
};

struct IfStmt : Stmt {
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> thenStmt;
	std::unique_ptr<Stmt> elseStmt;

	IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenStmt, std::unique_ptr<Stmt> elseStmt, size_t line) : Stmt(NodeKind::IfStmt, line), condition(std::move(condition)), thenStmt(std::move(thenStmt)), elseStmt(std::move(elseStmt)) {}
};

struct WhileStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body, size_t line)
        : Stmt(NodeKind::WhileStmt, line), condition(std::move(cond)), body(std::move(body)) {}
};

struct DoWhileStmt : Stmt {
    std::unique_ptr<Stmt> body;
    std::unique_ptr<Expr> condition;

    DoWhileStmt(std::unique_ptr<Stmt> body, std::unique_ptr<Expr> cond, size_t line)
        : Stmt(NodeKind::DoWhileStmt, line), body(std::move(body)), condition(std::move(cond)) {}
};


struct ForStmt : Stmt {
    std::unique_ptr<Stmt> init;
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> increment;
    std::unique_ptr<Stmt> body;

    ForStmt(std::unique_ptr<Stmt> init, std::unique_ptr<Expr> cond,
            std::unique_ptr<Expr> inc, std::unique_ptr<Stmt> body, size_t line)
        : Stmt(NodeKind::ForStmt, line), init(std::move(init)),
          condition(std::move(cond)), increment(std::move(inc)), body(std::move(body)) {}
};

struct GotoStmt : Stmt {
    std::string_view target;

    GotoStmt(std::string_view target, size_t line)
        : Stmt(NodeKind::GotoStmt, line), target(target) {}
};

struct ReturnStmt : Stmt {
    std::unique_ptr<Expr> value;

    ReturnStmt(std::unique_ptr<Expr> val, size_t line)
        : Stmt(NodeKind::ReturnStmt, line), value(std::move(val)) {}
};

struct Decl : ASTNode {
	using ASTNode::ASTNode;
};

struct VarDecl : Decl {
    std::unique_ptr<TypeSpec> type;
    std::string_view name;
    std::unique_ptr<Expr> initializer;

    VarDecl(std::unique_ptr<TypeSpec> type, std::string_view name,
            std::unique_ptr<Expr> init, size_t line)
        : Decl(NodeKind::VarDecl, line), type(std::move(type)),
          name(name), initializer(std::move(init)) {}
};


struct StructDecl : Decl {
    bool isUnion;
    std::optional<std::string_view> name;
    std::vector<std::unique_ptr<VarDecl>> members;
	bool isDefinition;

    StructDecl(bool isUnion, std::optional<std::string_view> name,
               std::vector<std::unique_ptr<VarDecl>> members, size_t line)
        : Decl(NodeKind::StructDecl, line), isUnion(isUnion),
          name(name), members(std::move(members)), isDefinition(!members.empty()) {}
};

struct Enumerator {
    std::string_view name;
    std::unique_ptr<Expr> value;
    size_t line;

    Enumerator(std::string_view name, std::unique_ptr<Expr> value, size_t line)
        : name(name), value(std::move(value)), line(line) {}
};

struct EnumDecl : Decl {
    std::optional<std::string_view> name;
    std::vector<std::unique_ptr<Enumerator>> enumerators;
    bool isDefinition;

    EnumDecl(std::optional<std::string_view> name,
             std::vector<std::unique_ptr<Enumerator>> enumerators, size_t line)
        : Decl(NodeKind::EnumDecl, line), name(name),
          enumerators(std::move(enumerators)),
          isDefinition(!enumerators.empty()) {}
};

struct FunctionDecl : Decl {
    std::unique_ptr<TypeSpec> returnType;
    std::string_view name;
    std::vector<std::unique_ptr<VarDecl>> parameters;
    std::unique_ptr<CompoundStmt> body;

    FunctionDecl(std::unique_ptr<TypeSpec> retType, std::string_view name,
                 std::vector<std::unique_ptr<VarDecl>> params,
                 std::unique_ptr<CompoundStmt> body, size_t line)
        : Decl(NodeKind::FunctionDecl, line), returnType(std::move(retType)),
          name(name), parameters(std::move(params)), body(std::move(body)) {}
};

struct TranslationUnit {
    std::vector<std::unique_ptr<Decl>> declarations;
};

class Parser {
public:
    explicit Parser(Lexer& lexer) : lexer(lexer),current(lexer.tokenize()),previous(std::nullopt) {
        advance();
    }

    std::unique_ptr<TranslationUnit> parse();

private:
    Lexer& lexer;
    Token current;
	std::optional<Token> previous;

    // Utility functions
    void advance();
    bool check(Type type) const;
    bool match(Type type);
    bool match(std::initializer_list<Type> types);
    Token consume(Type type, const char* message);
    void synchronize();

    // Parsing functions - Declarations
    std::unique_ptr<Decl> declaration();
    std::unique_ptr<FunctionDecl> functionDeclaration(std::unique_ptr<TypeSpec> type, std::string_view name);
    std::unique_ptr<VarDecl> variableDeclaration(std::unique_ptr<TypeSpec> type, std::string_view name);
    std::unique_ptr<TypeSpec> typeSpecifier();

    // Parsing functions - Statements
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<CompoundStmt> compoundStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<IfStmt> ifStatement();
    std::unique_ptr<WhileStmt> whileStatement();
    std::unique_ptr<ForStmt> forStatement();
    std::unique_ptr<ReturnStmt> returnStatement();
    std::unique_ptr<Stmt> breakStatement();
    std::unique_ptr<Stmt> continueStatement();

    // Parsing functions - Expressions (precedence climbing)
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> conditional();
    std::unique_ptr<Expr> logicalOr();
    std::unique_ptr<Expr> logicalAnd();
    std::unique_ptr<Expr> bitwiseOr();
    std::unique_ptr<Expr> bitwiseXor();
    std::unique_ptr<Expr> bitwiseAnd();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> relational();
    std::unique_ptr<Expr> shift();
    std::unique_ptr<Expr> additive();
    std::unique_ptr<Expr> multiplicative();
    std::unique_ptr<Expr> cast();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> postfix();
    std::unique_ptr<Expr> primary();

    // Helper functions
    bool isTypeSpecifier(Type type) const;
    int getPrecedence(Type op) const;
};
