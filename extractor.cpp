//  Created on: 21 de jan de 2025
//      Author: roger.moschiel

#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/CommandLine.h"

// Mais includes para extrais macros
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PPCallbacks.h"
#include "llvm/Support/raw_ostream.h"

#include <fstream>  // Para salvar arquivo


using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

// Função para salvar os dados extraídos nos arquivos
void saveToFile(const std::string &definition, unsigned startLine, unsigned endLine) {
    std::ofstream defFile("definition.txt", std::ios::trunc);
    std::ofstream lineFile("lines.txt", std::ios::trunc);
    
    if (defFile.is_open() && lineFile.is_open()) {
        defFile << definition;
        lineFile << startLine << ";" << endLine;
        
        defFile.close();
        lineFile.close();
    }

    std::exit(0);
}


// Opções para CLI
static cl::OptionCategory ToolCategory("extractor options");

static cl::opt<std::string> FunctionName(
    "function", cl::desc("Name of the function to extract"),
    cl::value_desc("function name"), cl::init(""));

static cl::opt<std::string> PrototypeName(
    "prototype", cl::desc("Name of the function prototype to extract"),
    cl::value_desc("prototype name"), cl::init(""));

static cl::opt<std::string> TypedefName(
    "typedef", cl::desc("Name of the typedef to extract"),
    cl::value_desc("typedef name"), cl::init(""));

static cl::opt<std::string> StructName(
    "struct", cl::desc("Name of the struct to extract"),
    cl::value_desc("struct name"), cl::init(""));

static cl::opt<std::string> EnumName(
    "enum", cl::desc("Name of the enum to extract"),
    cl::value_desc("enum name"), cl::init(""));

static cl::opt<std::string> UnionName(
    "union", cl::desc("Name of the union to extract"),
    cl::value_desc("union name"), cl::init(""));

static cl::opt<std::string> GlobalVarName(
    "global", cl::desc("Name of the global variable to extract"),
    cl::value_desc("global variable name"), cl::init(""));

static cl::opt<std::string> ExternFunctionName(
    "extern-function", cl::desc("Name of the extern function to extract"),
    cl::value_desc("extern function name"), cl::init(""));

static cl::opt<std::string> ExternVariableName(
    "extern-variable", cl::desc("Name of the extern variable to extract"),
    cl::value_desc("extern variable name"), cl::init(""));

static cl::opt<std::string> MacroName(
    "macro", cl::desc("Name of the macro to extract"),
    cl::value_desc("macro name"), cl::init(""));
        


// Callback para funções
class FunctionExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const FunctionDecl *Func = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl")) {
            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = Func->getBeginLoc();
            SourceLocation EndLoc = Func->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = Lexer::getSourceText(
                CharSourceRange::getTokenRange(Func->getSourceRange()), SM, LangOptions()).str() + "\n";

            llvm::outs() << "Function: " 
                         << Func->getNameInfo().getName().getAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                         << Definition;

            saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para protótipos de funções
class PrototypeExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const FunctionDecl *Func = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl")) {
            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = Func->getBeginLoc();
            SourceLocation EndLoc = Func->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = Lexer::getSourceText(
                CharSourceRange::getTokenRange(Func->getSourceRange()), SM, LangOptions()).str() + ";\n";

            llvm::outs() << "Prototype: " 
                            << Func->getNameInfo().getName().getAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                            << Definition;

                            saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para typedefs
class TypedefExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const TypedefDecl *TD = Result.Nodes.getNodeAs<TypedefDecl>("typedefDecl")) {
            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = TD->getBeginLoc();
            SourceLocation EndLoc = TD->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(TD->getSourceRange()), SM, LangOptions())) + ";\n";

            llvm::outs() << "Typedef: " << TD->getNameAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                         << Definition;

                         saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para structs
class StructExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const RecordDecl *RD = Result.Nodes.getNodeAs<RecordDecl>("structDecl")) {
            if (!RD->isStruct()) return;  // Filtra apenas structs

            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = RD->getBeginLoc();
            SourceLocation EndLoc = RD->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(RD->getSourceRange()), SM, LangOptions())) + ";\n";
            

            llvm::outs() << "Struct: " << RD->getNameAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                         << Definition;

                         saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para enums
class EnumExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const EnumDecl *ED = Result.Nodes.getNodeAs<EnumDecl>("enumDecl")) {
            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = ED->getBeginLoc();
            SourceLocation EndLoc = ED->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(ED->getSourceRange()), SM, LangOptions())) + ";\n";

            llvm::outs() << "Enum: " << ED->getNameAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                         << Definition;

                         saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para unions
class UnionExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const RecordDecl *RD = Result.Nodes.getNodeAs<RecordDecl>("unionDecl")) {
            if (!RD->isUnion()) return;  // Filtra apenas unions
            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = RD->getBeginLoc();
            SourceLocation EndLoc = RD->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(RD->getSourceRange()), SM, LangOptions())) + ";\n";

            llvm::outs() << "Union: " << RD->getNameAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                         << Definition;

                         saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para variaveis globais
class GlobalVariableExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("globalVarDecl")) {
            // Verifica se a variável tem armazenamento global
            if (!VD->hasGlobalStorage())
                return;

            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = VD->getBeginLoc();
            SourceLocation EndLoc = VD->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(VD->getSourceRange()), SM, LangOptions())) + ";\n";

            llvm::outs() << "Global Variable: " << VD->getNameAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                            << Definition;

                            saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para funções extern
class ExternFunctionExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("externFuncDecl")) {
            // Funções possuem ligação externa por padrão, mas podemos checar se realmente tem external linkage
            #if defined(__clang__) && __clang_major__ >= 15
            if (FD->getLinkageInternal() != Linkage::External)
                return;
            #else
            if (FD->getLinkageInternal() != ExternalLinkage)
                return;
            #endif

            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = FD->getBeginLoc();
            SourceLocation EndLoc = FD->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(FD->getSourceRange()), SM, LangOptions())) + ";\n";

            llvm::outs() << "Extern Function: " 
                            << FD->getNameInfo().getName().getAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                            << Definition;

                            saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para variáveis extern
class ExternVariableExtractorCallback : public MatchFinder::MatchCallback {
public:
    void run(const MatchFinder::MatchResult &Result) override {
        if (const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("externVarDecl")) {
            // Verifica se a variável tem storage class extern
            if (VD->getStorageClass() != SC_Extern)
                return;

            SourceManager &SM = *Result.SourceManager;
            SourceLocation StartLoc = VD->getBeginLoc();
            SourceLocation EndLoc = VD->getEndLoc();

            unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
            unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
            std::string Definition = std::string(Lexer::getSourceText(CharSourceRange::getTokenRange(VD->getSourceRange()), SM, LangOptions())) + ";\n";

            llvm::outs() << "Extern Variable: " << VD->getNameAsString() << "\n";
            llvm::outs() << "Start Line: " << StartLine << "\n";
            llvm::outs() << "End Line: " << EndLine << "\n";
            llvm::outs() << "Definition:\n"
                            << Definition;

                            saveToFile(Definition, StartLine, EndLine);
        }
    }
};

// Callback para capturar definições de macro
class MacroExtractorCallback : public PPCallbacks {
public:
    MacroExtractorCallback(SourceManager &SM, const std::string &TargetMacro)
        : SM(SM), TargetMacro(TargetMacro) {}

    void MacroDefined(const Token &MacroNameToken, const MacroDirective *MD) override {
        IdentifierInfo *II = MacroNameToken.getIdentifierInfo();
        if (!II)
            return;
        std::string DefinedMacro = II->getName().str();
        if (DefinedMacro != TargetMacro)
            return;
        const MacroInfo *MI = MD->getMacroInfo();
        if (!MI)
            return;
        // Obter o início da definição da macro
        SourceLocation StartLoc = MI->getDefinitionLoc();
        SourceLocation EndLoc;
        ArrayRef<Token> Tokens = MI->tokens();
        if (!Tokens.empty()) {
            // Para macros, pegue o final do último token
            EndLoc = Tokens.back().getEndLoc();
        } else {
            EndLoc = MI->getDefinitionLoc();
        }

        unsigned StartLine = SM.getSpellingLineNumber(StartLoc);
        unsigned EndLine = SM.getSpellingLineNumber(EndLoc);
        std::string Definition = "";

        CharSourceRange Range = CharSourceRange::getCharRange(StartLoc, EndLoc);
        StringRef MacroText = Lexer::getSourceText(Range, SM, LangOptions());

        if(MacroText.empty())
            Definition = "#define " + DefinedMacro + "\n";
        else
            Definition = std::string("#define") + MacroText.str() + "\n";

        llvm::outs() << "Macro: " << DefinedMacro << "\n";
        llvm::outs() << "Start Line: " << StartLine << "\n";
        llvm::outs() << "End Line: " << EndLine << "\n";
        llvm::outs() << "Definition:\n"
                            << Definition;

                            saveToFile(Definition, StartLine, EndLine);
    }

private:
    SourceManager &SM;
    std::string TargetMacro;
};

// FrontendAction customizada para extrair macros
class MacroExtractorAction : public ASTFrontendAction {
public:
    MacroExtractorAction(const std::string &MacroName) : TargetMacro(MacroName) {}

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        // Registra o callback para capturar macros no pré-processador
        CI.getPreprocessor().addPPCallbacks(std::make_unique<MacroExtractorCallback>(CI.getSourceManager(), TargetMacro));
        // Retorna um ASTConsumer vazio, pois não precisamos processar o AST
        return std::make_unique<ASTConsumer>();
    }

private:
    std::string TargetMacro;
};

// Factory customizada para MacroExtractorAction
class MacroFrontendActionFactory : public FrontendActionFactory {
public:
    MacroFrontendActionFactory(const std::string &MacroName) : TargetMacro(MacroName) {}

    std::unique_ptr<FrontendAction> create() override {
        return std::make_unique<MacroExtractorAction>(TargetMacro);
    }

private:
    std::string TargetMacro;
};

int main(int argc, const char **argv) {
    // Cria o OptionsParser; ele processa os argumentos e extra-args (como -extra-arg) automaticamente.
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, ToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << "Erro ao criar OptionsParser.\n";
        return 1;
    }
    CommonOptionsParser &OptionsParser = ExpectedParser.get();

    // Cria o ClangTool usando o CompilationDatabase e a lista de arquivos obtidos pelo OptionsParser.
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    
    // Insere o argumento "-fsyntax-only" ao final dos argumentos.
    Tool.appendArgumentsAdjuster(getInsertArgumentAdjuster({"-fsyntax-only"}, ArgumentInsertPosition::END));

    // Se a opção -macro foi fornecida, utiliza a ação customizada para macro
    if (!MacroName.empty()) {
        MacroFrontendActionFactory MacroFactory(MacroName);
        return Tool.run(&MacroFactory);
    }
    else
    {
        // Se não estiver extraindo macro, pode executar outra ação, como a SyntaxOnlyAction

        // Configura os callbacks e o matcher.
        FunctionExtractorCallback FuncCallback;
        PrototypeExtractorCallback PrototypeCallback;
        TypedefExtractorCallback TypedefCallback;
        StructExtractorCallback StructCallback;
        EnumExtractorCallback EnumCallback;
        UnionExtractorCallback UnionCallback;
        GlobalVariableExtractorCallback GlobalVarCallback;
        ExternFunctionExtractorCallback ExternFuncCallback;
        ExternVariableExtractorCallback ExternVarCallback;
    
        MatchFinder Finder;
    
        if (!FunctionName.empty()) {
            Finder.addMatcher(functionDecl(hasName(FunctionName), isDefinition(), isExpansionInMainFile()).bind("funcDecl"), &FuncCallback);
        }
        if (!PrototypeName.empty()) {
            Finder.addMatcher(functionDecl(hasName(PrototypeName), unless(isDefinition()), isExpansionInMainFile()).bind("funcDecl"), &PrototypeCallback);
        }
        if (!TypedefName.empty()) {
            Finder.addMatcher(typedefDecl(hasName(TypedefName), isExpansionInMainFile()).bind("typedefDecl"), &TypedefCallback);
        }
        if (!StructName.empty()) {
            Finder.addMatcher(recordDecl(hasName(StructName), isStruct(), isExpansionInMainFile()).bind("structDecl"), &StructCallback);
        }
        if (!EnumName.empty()) {
            Finder.addMatcher(enumDecl(hasName(EnumName), isExpansionInMainFile()).bind("enumDecl"), &EnumCallback);
        }
        if (!UnionName.empty()) {
            Finder.addMatcher(recordDecl(hasName(UnionName), isUnion(), isExpansionInMainFile()).bind("unionDecl"), &UnionCallback);
        }
        if (!GlobalVarName.empty()) {
            Finder.addMatcher(
                varDecl(hasGlobalStorage(), hasName(GlobalVarName), isExpansionInMainFile()).bind("globalVarDecl"),
                &GlobalVarCallback);
        }
        if (!ExternFunctionName.empty()) {
            Finder.addMatcher(
                functionDecl(hasName(ExternFunctionName), isExpansionInMainFile()).bind("externFuncDecl"),
                &ExternFuncCallback);
        }
        if (!ExternVariableName.empty()) {
            Finder.addMatcher(
                varDecl(hasName(ExternVariableName), isExpansionInMainFile()).bind("externVarDecl"),
                &ExternVarCallback);
        }
    
        return Tool.run(newFrontendActionFactory(&Finder).get());
    }
}




