#pragma once

// Coat compares unfortunately return a coat condition, meaning its value can't be checked
// Inherits from coat::Value<::asmjit::x86::Compiler, CC> and overrides the > operator
// Only this operator is necessary for the project

// Also unfortunately coat marks Value as final
// TODO define class CoatValue and classes CoatAsmjitValue & CoatLLVMValue inheriting from it
template<typename T>
struct CoatAsmjitValue<::asmjit::x86::Compiler,T> final : public coat::Value<::asmjit::x86::Compiler,T> {

};