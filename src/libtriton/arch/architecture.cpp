//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the Apache License 2.0.
*/

#include <new>

#include <triton/aarch64Cpu.hpp>
#include <triton/architecture.hpp>
#include <triton/arm32Cpu.hpp>
#include <triton/exceptions.hpp>
#include <triton/x8664Cpu.hpp>
#include <triton/x86Cpu.hpp>



namespace triton {
  namespace arch {

    Architecture::Architecture(triton::callbacks::Callbacks* callbacks) {
      this->arch      = triton::arch::ARCH_INVALID;
      this->callbacks = callbacks;
    }


    triton::arch::architecture_e Architecture::getArchitecture(void) const {
      return this->arch;
    }


    triton::arch::endianness_e Architecture::getEndianness(void) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getEndianness(): You must define an architecture.");
      return this->cpu->getEndianness();
    }


    triton::arch::CpuInterface* Architecture::getCpuInstance(void) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getCpuInstance(): CPU undefined.");
      return this->cpu.get();
    }


    void Architecture::setArchitecture(triton::arch::architecture_e arch) {
      /* Allocate and init the good arch */
      switch (arch) {
        case triton::arch::ARCH_X86_64:
          /* init the new instance */
          this->cpu.reset(new(std::nothrow) triton::arch::x86::x8664Cpu(this->callbacks));
          if (this->cpu == nullptr)
            throw triton::exceptions::Architecture("Architecture::setArchitecture(): Not enough memory.");
          break;

        case triton::arch::ARCH_X86:
          /* init the new instance */
          this->cpu.reset(new(std::nothrow) triton::arch::x86::x86Cpu(this->callbacks));
          if (this->cpu == nullptr)
            throw triton::exceptions::Architecture("Architecture::setArchitecture(): Not enough memory.");
          break;

        case triton::arch::ARCH_AARCH64:
          /* init the new instance */
          this->cpu.reset(new(std::nothrow) triton::arch::arm::aarch64::AArch64Cpu(this->callbacks));
          if (this->cpu == nullptr)
            throw triton::exceptions::Architecture("Architecture::setArchitecture(): Not enough memory.");
          break;

        case triton::arch::ARCH_ARM32:
          /* init the new instance */
          this->cpu.reset(new(std::nothrow) triton::arch::arm::arm32::Arm32Cpu(this->callbacks));
          if (this->cpu == nullptr)
            throw triton::exceptions::Architecture("Architecture::setArchitecture(): Not enough memory.");
          break;

        default:
          throw triton::exceptions::Architecture("Architecture::setArchitecture(): Architecture not supported.");
          break;
      }

      /* Setup global variables */
      this->arch = arch;
    }


    void Architecture::clearArchitecture(void) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::clearArchitecture(): You must define an architecture.");
      this->cpu->clear();
    }


    bool Architecture::isValid(void) const {
      if (this->arch == triton::arch::ARCH_INVALID)
        return false;
      return true;
    }


    bool Architecture::isFlag(triton::arch::register_e regId) const {
      if (!this->cpu)
        return false;
      return this->cpu->isFlag(regId);
    }


    bool Architecture::isFlag(const triton::arch::Register& reg) const {
      return this->isFlag(reg.getId());
    }


    bool Architecture::isRegister(triton::arch::register_e regId) const {
      if (!this->cpu)
        return false;
      return this->cpu->isRegister(regId);
    }


    bool Architecture::isRegister(const triton::arch::Register& reg) const {
      return this->isRegister(reg.getId());
    }


    bool Architecture::isRegisterValid(triton::arch::register_e regId) const {
      if (!this->cpu)
        return false;
      return this->cpu->isRegisterValid(regId);
    }


    bool Architecture::isRegisterValid(const triton::arch::Register& reg) const {
      return this->isRegisterValid(reg.getId());
    }


    bool Architecture::isThumb(void) const {
      if (!this->cpu)
        return false;
      return this->cpu->isThumb();
    }


    void Architecture::setThumb(bool state) {
      if (this->cpu) {
        this->cpu->setThumb(state);
      }
    }


    bool Architecture::isMemoryExclusiveAccess(void) const {
      if (!this->cpu)
        return false;
      return this->cpu->isMemoryExclusiveAccess();
    }


    void Architecture::setMemoryExclusiveAccess(bool state) {
      if (this->cpu) {
        this->cpu->setMemoryExclusiveAccess(state);
      }
    }


    triton::uint32 Architecture::numberOfRegisters(void) const {
      if (!this->cpu)
        return 0;
      return this->cpu->numberOfRegisters();
    }


    triton::uint32 Architecture::gprSize(void) const {
      if (!this->cpu)
        return 0;
      return this->cpu->gprSize();
    }


    triton::uint32 Architecture::gprBitSize(void) const {
      if (!this->cpu)
        return 0;
      return this->cpu->gprBitSize();
    }


    const std::unordered_map<triton::arch::register_e, const triton::arch::Register>& Architecture::getAllRegisters(void) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getAllRegisters(): You must define an architecture.");
      return this->cpu->getAllRegisters();
    }


    std::set<const triton::arch::Register*> Architecture::getParentRegisters(void) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getParentRegisters(): You must define an architecture.");
      return this->cpu->getParentRegisters();
    }


    const triton::arch::Register& Architecture::getProgramCounter(void) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getProgramCounter(): You must define an architecture.");
      return this->cpu->getProgramCounter();
    }


    const triton::arch::Register& Architecture::getStackPointer(void) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getStackPointer(): You must define an architecture.");
      return this->cpu->getStackPointer();
    }


    const triton::arch::Register& Architecture::getRegister(triton::arch::register_e id) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getRegister(): You must define an architecture.");
      return this->cpu->getRegister(id);
    }


    const triton::arch::Register& Architecture::getRegister(const std::string& name) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getRegister(): You must define an architecture.");
      return this->cpu->getRegister(name);
    }


    const triton::arch::Register& Architecture::getParentRegister(const triton::arch::Register& reg) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getParentRegister(): You must define an architecture.");
      return this->cpu->getParentRegister(reg);
    }


    const triton::arch::Register& Architecture::getParentRegister(triton::arch::register_e id) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getParentRegister(): You must define an architecture.");
      return this->cpu->getParentRegister(id);
    }


    void Architecture::disassembly(triton::arch::Instruction& inst) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::disassembly(): You must define an architecture.");
      this->cpu->disassembly(inst);
    }


    std::vector<triton::arch::Instruction> Architecture::disassembly(triton::uint64 addr, triton::usize count) const {
      std::vector<triton::arch::Instruction> ret;
      ret.reserve(count);

      while (count--) {
        if (!this->isConcreteMemoryValueDefined(addr)) {
          break;
        }
        auto opcodes = this->getConcreteMemoryAreaValue(addr, 16);
        auto inst = triton::arch::Instruction(addr, reinterpret_cast<triton::uint8*>(opcodes.data()), opcodes.size());
        this->disassembly(inst);
        ret.push_back(inst);
        addr += inst.getSize();
      }

      return ret;
    }


    std::vector<triton::arch::Instruction> Architecture::disassembly(triton::uint64 addr) const {
      std::vector<triton::arch::Instruction> ret;

      do {
        if (!this->isConcreteMemoryValueDefined(addr)) {
          break;
        }
        auto opcodes = this->getConcreteMemoryAreaValue(addr, 16);
        auto inst = triton::arch::Instruction(addr, reinterpret_cast<triton::uint8*>(opcodes.data()), opcodes.size());
        this->disassembly(inst);
        ret.push_back(inst);
        addr += inst.getSize();
      } while (!ret.back().isControlFlow());

      return ret;
    }


    triton::uint8 Architecture::getConcreteMemoryValue(triton::uint64 addr, bool execCallbacks) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteMemoryValue(): You must define an architecture.");
      return this->cpu->getConcreteMemoryValue(addr, execCallbacks);
    }


    triton::uint512 Architecture::getConcreteMemoryValue(const triton::arch::MemoryAccess& mem, bool execCallbacks) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteMemoryValue(): You must define an architecture.");
      return this->cpu->getConcreteMemoryValue(mem, execCallbacks);
    }


    std::vector<triton::uint8> Architecture::getConcreteMemoryAreaValue(triton::uint64 baseAddr, triton::usize size, bool execCallbacks) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteMemoryAreaValue(): You must define an architecture.");
      return this->cpu->getConcreteMemoryAreaValue(baseAddr, size, execCallbacks);
    }


    triton::uint512 Architecture::getConcreteRegisterValue(const triton::arch::Register& reg, bool execCallbacks) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::getConcreteRegisterValue(): You must define an architecture.");
      return this->cpu->getConcreteRegisterValue(reg, execCallbacks);
    }


    void Architecture::setConcreteMemoryValue(triton::uint64 addr, triton::uint8 value) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryValue(addr, value);
    }


    void Architecture::setConcreteMemoryValue(const triton::arch::MemoryAccess& mem, const triton::uint512& value) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryValue(mem, value);
    }


    void Architecture::setConcreteMemoryAreaValue(triton::uint64 baseAddr, const std::vector<triton::uint8>& values) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryAreaValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryAreaValue(baseAddr, values);
    }


    void Architecture::setConcreteMemoryAreaValue(triton::uint64 baseAddr, const triton::uint8* area, triton::usize size) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteMemoryAreaValue(): You must define an architecture.");
      this->cpu->setConcreteMemoryAreaValue(baseAddr, area, size);
    }


    void Architecture::setConcreteRegisterValue(const triton::arch::Register& reg, const triton::uint512& value) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::setConcreteRegisterValue(): You must define an architecture.");
      this->cpu->setConcreteRegisterValue(reg, value);
    }


    bool Architecture::isConcreteMemoryValueDefined(const triton::arch::MemoryAccess& mem) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::isConcreteMemoryValueDefined(): You must define an architecture.");
      return this->cpu->isConcreteMemoryValueDefined(mem);
    }


    bool Architecture::isConcreteMemoryValueDefined(triton::uint64 baseAddr, triton::usize size) const {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::isConcreteMemoryValueDefined(): You must define an architecture.");
      return this->cpu->isConcreteMemoryValueDefined(baseAddr, size);
    }


    void Architecture::clearConcreteMemoryValue(const triton::arch::MemoryAccess& mem) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::clearConcreteMemoryValue(): You must define an architecture.");
      this->cpu->clearConcreteMemoryValue(mem);
    }


    void Architecture::clearConcreteMemoryValue(triton::uint64 baseAddr, triton::usize size) {
      if (!this->cpu)
        throw triton::exceptions::Architecture("Architecture::clearConcreteMemoryValue(): You must define an architecture.");
      this->cpu->clearConcreteMemoryValue(baseAddr, size);
    }

  }; /* arch namespace */
}; /* triton namespace */
