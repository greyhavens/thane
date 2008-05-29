/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


#include "avmplus.h"

namespace avmplus
{
	using namespace MMgc;

#ifdef AVMPLUS_AMD64
#ifdef AVMPLUS_VERBOSE

	const char *CodegenMIR::gpregNames[] = {		
		"eax",
		"ecx",
		"edx",
		"ebx",
		"esp",
		"ebp",
		"esi",
		"edi",
		"rax",
		"rcx",
		"rdx",
		"rbx",
		"rsp",
		"rbp",
		"rsi",
		"rdi",
		"r8",
		"r9",
		"r10",
		"r11",
		"r12",
		"r13",
		"r14",
		"r15"		
		
	};

	const char *CodegenMIR::xmmregNames[] = {
		"xmm0",
		"xmm1",
		"xmm2",
		"xmm3",
		"xmm4",
		"xmm5",
		"xmm6",
		"xmm7"
		"xmm8",
		"xmm9",
		"xmm10",
		"xmm11",
		"xmm12",
		"xmm13",
		"xmm14",
		"xmm15"
	};

	const char *CodegenMIR::x87regNames[] = {
		"fst0",
		"fst1",
		"fst2",
		"fst3",
		"fst4",
		"fst5",
		"fst6",
		"fst7",
	};			


#endif // AVMPLUS_VERBOSE

	void CodegenMIR::REX(Register a, Register b, bool set64bit)
	{
		// Needs REX prefix byte since we are working with a 64-bit register
		// bits 7:4 - 0100 - 0x40
		// bit 3 - Write - 1 implies 64-bit operand size
		// bit 2 - Read - Extension of the ModR/M reg field
		// bit 1 - X - Extension of the SIB index field
		// bit 0 - B - Extension of the ModR/M r/m field, SIB base field or Opcode reg field 
		AvmAssert (RAX == 8);
		AvmAssert (R8 == 16);
		#if 0 // not sure about this - okay in some circumstances but not others?
		// both r and base can't be r8-r15.  Mac ASM code shows this as two instructions:
		// Instead of 'mov r12, [r11+4]' it generates 'mov rax, r11; mov r12, [rax+4]'
		AvmAssert ((a < 16) || (b < 16));
		#endif
				
		if ((a >= 8) || (b >= 8))
		{
			int val = 0x40;
			if (set64bit)
				val	|= 0x08;
			if (a >= 16)
				val |= 0x04;
			if (b >= 16) 
				val |= 0x01;
				
			*mip++ = (MDInstruction)val; 
		}	
	}

	void CodegenMIR::MODRM(Register reg, Register operand)
	{
		// Ignore upper part for RAX to R15
		reg = (Register)(int(reg) & 0x7);
		operand = (Register)(int(operand) & 0x7);
	
 		*mip++ = (MDInstruction)(3<<6 | reg<<3 | operand);
 	}

	void CodegenMIR::MODRM(Register reg, sintptr disp, Register base, int lshift, Register index)
 	{
		// Ignore upper part for RAX to R15
		reg = (Register)(int(reg) & 0x7);
		base = (Register)(int(base) & 0x7);
		index = (Register)(int(index) & 0x7);
	
 		// reg <-> disp[base+index<<lshift]
 		AvmAssert(lshift >= 0 && lshift <= 3);
 		if (disp == 0 && base != EBP) {
 			mip[0] =      (MDInstruction)(0<<6 |   reg<<3 | 4);		// ModR/M
 			mip[1] = (MDInstruction)(lshift<<6 | index<<3 | base);	// SIB
			mip += 2;
 		} else if (is8bit((int)disp)) {
 			mip[0] =      (MDInstruction)(1<<6 |   reg<<3 | 4);		// ModR/M
 			mip[1] = (MDInstruction)(lshift<<6 | index<<3 | base);	// SIB
			mip[2] = (MDInstruction)disp;
 			mip += 3;
 		} else if (is32bit(disp)) {
 			*(mip++) =      (MDInstruction)(2<<6 |   reg<<3 | 4);		// ModR/M
 			*(mip++) = (MDInstruction)(lshift<<6 | index<<3 | base);		// SIB
 			IMM32((int32)disp);
 		}
		else
		{
			AvmAssert(0);
		}
 	}

	void CodegenMIR::MODRM(Register reg, sintptr disp, Register base)
 	{
		// Ignore upper part for RAX to R15
		reg = (Register)(int(reg) & 0x7);
		if (base != Unknown)
			base = (Register)(int(base) & 0x7);


 		// dest <-> disp[base]
		if (base == Unknown) {
			// In 64bit mode, we need to encode a SIB byte to do 32-bit 
			// absolute addressing, instead of RIP addressing
			if (is32bit (disp))
			{
				// disp = absolute addr
				*(mip++) = (MDInstruction)(0<<6 | reg<<3 | 4); // ModR/M
				*(mip++) = (MDInstruction)( 0<<6 | 4<<3 | 5); // SIB
				IMM32((int32)disp);
			}
			else
			{
				AvmAssert(0);
			}
		}
 		else if (base == ESP) {
 			MODRM(reg, disp, base, 0, (Register)4); // index==4 means ignore index
 		}
		else if (disp == 0 && base != EBP) {
 			*(mip++) = (MDInstruction)(0<<6 | reg<<3 | base); // mod r/m
 		} else if (is8bit((int)disp)) {
 			*(mip++) = (MDInstruction)(1<<6 | reg<<3 | base); // mod r/m
 			*(mip++) = (MDInstruction)disp;
 		} else if (is32bit(disp)) {
 			*(mip++) = (MDInstruction)(2<<6 | reg<<3 | base); // mod r/m
 			IMM32((int32)disp);
 		}
		else		
		{
			AvmAssert(0);
		}
 	}

	void CodegenMIR::PUSH(Register r)
	{
		int reg = int(r);
		AvmAssert (reg >= 8); // lower registers are not supported in 64-bit mode
		if (r >= 16)
			*mip++ = 0x41;
			
		ALU(0x50 | (reg & 0x7));
	}
	
	void CodegenMIR::POP(Register r)
	{
		int reg = int(r);
		AvmAssert (reg >= 8); // lower registers are not supported in 64-bit mode
		if (r >= 16)
			*mip++ = 0x41;
			
		ALU(0x58 | (reg & 0x7));
	}

	void CodegenMIR::PUSH(sintptr imm)
 	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  push  %d\n", mip, imm);
		#endif /* AVMPLUS_VERBOSE */

		if (is8bit((int)imm)) {
			// push imm8 (sign extended)
			mip[0] = (MDInstruction)(0x6a);
			mip[1] = (MDInstruction)(imm);
			mip += 2;
 		} else if (is32bit (imm)) {
			// push imm32
 			*mip++ = 0x68;
 			IMM32((int32)imm);
 		}
		else
		{
			AvmAssert(0);
		}
 	}

	void CodegenMIR::MOV(Register dest, sintptr imm) 
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  mov   %R, %D\n", mip, dest, imm);
		#endif /* AVMPLUS_VERBOSE */

		if (dest < 8)
		{
			if (is32bit(imm))
			{
				// mov reg, imm
				*mip++ = (MDInstruction)(0xb8 | dest);
				IMM32((int)imm);
			}
			else
			{
				AvmAssert(0);
			}	
		}
		else // 64-bit register
		{
			REX(RAX, dest);
			*mip++ = (MDInstruction)(0xb8 | (int(dest) & 0x7));
			IMM64(imm);
		}
	}

	void CodegenMIR::MOV(sintptr disp, Register base, sintptr imm, bool force32) 
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  mov   %d(%R), %d\n", mip, disp, base, imm);
		#endif /* AVMPLUS_VERBOSE */

		if (is32bit(imm))
		{
			if (base >= 8)
				REX(RAX, base, !force32);

			// mov disp[base], imm32
			*mip++ = 0xc7;
			MODRM((Register)0, disp, base);
			IMM32((int32)imm);
		}
		else
		{
			AvmAssert(0);
		}
	}

	void CodegenMIR::ALU (byte op, Register reg, sintptr imm)
 	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			const char *opstr="?";
			switch(op) {
			case 0x2d: opstr = "sub  "; break;
			case 0x05: opstr = "add  "; break;
			case 0x25: opstr = "and  "; break;
			case 0x23: opstr = "and  "; break;
			case 0x35: opstr = "xor  "; break;
			case 0x0d: opstr = "or   "; break;
			case 0x3d: opstr = "cmp  "; break;
			}
			core->console.format("    %A  %s %R, %d\n", mip, opstr, reg, imm);
		}
		#endif

		// REX is odd here, because the Register is in ModRM r/m, not reg
		REX(Unknown, reg, false);
		reg = (Register)(int(reg) & 0x7);
				
 		if (is8bit((int)imm)) {
			// <op> reg, imm8
 			mip[0] = (MDInstruction)(0x83);
 			mip[1] = (MDInstruction)(3<<6 | op&~7 | reg);
 			mip[2] = (MDInstruction)imm;
			mip+=3;
 		} else if (is32bit(imm)) {
 			if (reg == RAX) {
				// <op> RAX, imm32
 				*mip++ = op;
 			} else {
				// <op> reg, imm32
 				mip[0] = (MDInstruction)(0x81);
 				mip[1] = (MDInstruction)(3<<6 | op&~7 | reg);
				mip+=2;
 			}
 			IMM32((int32)imm);
 		}
		else
		{
			AvmAssert(0);
		}
 	}

	void CodegenMIR::ALU64(int op, Register r, Register rhs)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch(op) {
			case 0x03: core->console.format("    %A  add   %R, %R\n", mip, r, rhs); break;
			case 0x23: core->console.format("    %A  and   %R, %R\n", mip, r, rhs); break;
			case 0x87: core->console.format("    %A  xchg  %R, %R\n", mip, r, rhs); break;
			case 0x8b: core->console.format("    %A  mov   %R, %R\n", mip, r, rhs); break;
			}
		}
		#endif /* AVMPLUS_VERBOSE */

		REX(r, rhs, true);

		*mip++ = (MDInstruction)op; 
		MODRM(r, rhs);
	}

	void CodegenMIR::ALU(int op, Register r, Register rhs)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch(op) {
			case 0x2b: core->console.format("    %A  sub   %R, %R\n", mip, r, rhs); break;
			case 0x03: core->console.format("    %A  add   %R, %R\n", mip, r, rhs); break;
			case 0x23: core->console.format("    %A  and   %R, %R\n", mip, r, rhs); break;
			case 0x33: core->console.format("    %A  xor   %R, %R\n", mip, r, rhs); break;
			case 0x0b: core->console.format("    %A  or    %R, %R\n", mip, r, rhs); break;
			case 0x3b: core->console.format("    %A  cmp   %R, %R\n", mip, r, rhs); break;
			case 0xaf: core->console.format("    %A  imul  %R, %R\n", mip, r, rhs); break;
			case 0x85: core->console.format("    %A  test  %R, %R\n", mip, r, rhs); break;
			case 0xf7: core->console.format("    %A  neg   %R\n", mip, rhs); break;
			case 0x87: core->console.format("    %A  xchg  %R, %R\n", mip, r, rhs); break;
			case 0x8b: core->console.format("    %A  mov   %R, %R\n", mip, r, rhs); break;
			case 0x63: core->console.format("    %A  movsxd  %R, %R\n", mip, r, rhs); break;
			}
		}
		#endif /* AVMPLUS_VERBOSE */

		// SHR/SAR and SAL/SHL need to mask the shift operand to 
		// 5 bits, to make a 31-bit shift maximum
		// shift op is always in RCX
			REX(r, rhs, false);

		*mip++ = (MDInstruction)op; 
		MODRM(r, rhs);
	}

	void CodegenMIR::ALU2(int op, Register r, Register rhs)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch(op) {
			case 0x0faf: core->console.format("    %A  imul  %R, %R\n", mip, r, rhs); break;
			case 0x0f92: core->console.format("    %A  setb  %R\n", mip, r); break;
			case 0x0f93: core->console.format("    %A  setnb %R\n", mip, r); break;
			case 0x0f94: core->console.format("    %A  sete  %R\n", mip, r); break;
			case 0x0f95: core->console.format("    %A  setne %R\n", mip, r); break;
			case 0x0f96: core->console.format("    %A  setbe %R\n", mip, r); break;
			case 0x0f97: core->console.format("    %A  setnbe %R\n", mip, r); break;
			case 0x0f9a: core->console.format("    %A  setp  %R\n", mip, r); break;
			case 0x0f9b: core->console.format("    %A  setnp %R\n", mip, r); break;
			case 0x0f9C: core->console.format("    %A  setl  %R\n", mip, r); break;
			case 0x0f9E: core->console.format("    %A  setle %R\n", mip, r); break;
			case 0x0fb6: core->console.format("    %A  movzx_r8 %R, %R\n", mip, r, rhs);
			}
		}
		#endif /* AVMPLUS_VERBOSE */

		REX(r, rhs);

		mip[0] = (MDInstruction)(op>>8);
		mip[1] = (MDInstruction)op; 
		mip += 2;
		MODRM(r, rhs);
	}

	void CodegenMIR::SSE(int op, Register dest, Register src)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch(op) {
			case 0xf20f58: core->console.format("    %A  addsd %F, %F\n", mip, dest, src); break;
			case 0xf20f5c: core->console.format("    %A  subsd %F, %F\n", mip, dest, src); break;
			case 0xf20f59: core->console.format("    %A  mulsd %F, %F\n", mip, dest, src); break;
			case 0xf20f5e: core->console.format("    %A  divsd %F, %F\n", mip, dest, src); break;
			case 0xf20f2a: core->console.format("    %A  cvtsi2sd %F, %R\n", mip, dest, src); break;
			case 0x660f28: core->console.format("    %A  movapd %F, %F\n", mip, dest, src); break;
			case 0x660f2e: core->console.format("    %A  ucomisd %F, %F\n", mip, dest, src); break;
			case 0x660f6e: core->console.format("    %A  movd %F, %F\n", mip, dest, src); break;
			case 0xf20f10: core->console.format("    %A  movsd %F, %F)\n", mip, dest, src); break;
			}
		}
		#endif /* AVMPLUS_VERBOSE */

 		*mip++ = (MDInstruction)(op>>16);

		// Destination reg is always going to be <8 (XMM0-XMM7)
		if (src >= 8)
		{
			switch (op) {
				case 0xf20f2a:
				case 0x660f6e:
				case 0xf20f10:
					REX(dest, src, true);
					break;
				default:
					AvmAssertMsg(0, "Need REX byte");
			}
		}

		mip[0] = (MDInstruction)(op>>8);
		mip[1] = (MDInstruction)op;
		mip += 2;
		MODRM(dest, src);
	}

	void CodegenMIR::SSE(int op, Register r, sintptr disp, Register base)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch(op) {
			case 0xf20f58: core->console.format("    %A  addsd %F, %d(%R)\n", mip, r, disp, base); break;
			case 0xf20f5c: core->console.format("    %A  subsd %F, %d(%R)\n", mip, r, disp, base); break;
			case 0xf20f59: core->console.format("    %A  mulsd %F, %d(%R)\n", mip, r, disp, base); break;
			case 0xf20f5e: core->console.format("    %A  divsd %F, %d(%R)\n", mip, r, disp, base); break;
			case 0xf20f10: core->console.format("    %A  movsd %F, %d(%R)\n", mip, r, disp, base); break;
			case 0xf20f11: core->console.format("    %A  movsd %d(%R), %F\n", mip, disp, base, r); break;
			case 0xf20f2a: core->console.format("    %A  cvtsi2sd %F, %d(%R)\n", mip, r, disp, base); break;
			}
		}
		#endif /* AVMPLUS_VERBOSE */
		
			
		if (!is32bit(disp))
			AvmAssertMsg (0, "need support for 64-bit displacement?");

 		*mip++ = (MDInstruction)(op>>16);

		if ((r >= 8) || (base >= 16))
			REX(r, base, false);
		
		mip[0] = (MDInstruction)(op>>8);
		mip[1] = (MDInstruction)op;
		mip += 2;
 		MODRM(r, disp, base);
	}
	
	void CodegenMIR::XORPD(Register dest, uintptr addr) 
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  xorpd %F, %A\n", mip, dest, addr);
		#endif /* AVMPLUS_VERBOSE */

		if (dest >= 8)
			AvmAssertMsg (0, "need support for REX byte?");

		if (!is32bit(addr))
			AvmAssertMsg (0, "need support for 64-bit addr?");

		// xorpd dest, m128
 		mip[0] = 0x66;
		mip[1] = 0x0f;
		mip[2] = 0x57;
		mip[3] = (MDInstruction)(0<<6 | dest<<3 | 4); // ModR/M
		mip[4] = (MDInstruction)( 0<<6 | 4<<3 | 5); // SIB
		mip += 5;
		IMM32((int)addr);
	}

	void CodegenMIR::XORPD(Register dest, Register src) 
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  xorpd %F, %F\n", mip, dest, src);
		#endif /* AVMPLUS_VERBOSE */

		if (dest >= 8)
			AvmAssertMsg (0, "need support for REX byte?");

		// xorpd dest, src
 		mip[0] = 0x66;
		mip[1] = 0x0f;
		mip[2] = 0x57;
		mip[3] = (MDInstruction)(0xC0 | (dest<<3) | src);
		mip += 4;
	}
	void CodegenMIR::IMUL(Register dst, sintptr imm)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  imul  %R, %d\n", mip, dst, imm);
		#endif /* AVMPLUS_VERBOSE */

		REX(dst, dst, false);

		if (is8bit((int)imm))
		{
			*mip++ = 0x6b;
			MODRM(dst,dst);
			*mip++ = (MDInstruction)imm;
		}
		else if (is32bit(imm))
		{
			*mip++ = 0x69;
			MODRM(dst, dst);
			IMM32((int32)imm);
		}
		else
		{
			AvmAssert(0);
		}
	}

	void CodegenMIR::ALU(int op)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch(op) {
			case 0xc9: core->console.format("    %A  leave\n", mip); break;
			case 0xc3: core->console.format("    %A  ret\n", mip); break;
			case 0x90: core->console.format("    %A  nop\n", mip); break;
			case 0x9f: core->console.format("    %A  lahf\n", mip); break;
			case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
				core->console.format("    %A  push  %R\n", mip, op&7);
				break;
			case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F:
				core->console.format("    %A  pop   %R\n", mip, op&7);
				break;
			}
		}
		#endif /* AVMPLUS_VERBOSE */
		*mip++ = (MDInstruction)op;
	}

	void CodegenMIR::SHIFT(int op, Register r, int imm8)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			const char *opstr = "?";
			switch (op) {
			case 7: opstr = "sar  "; break;
			case 5: opstr = "shr  "; break;
			case 4: opstr = "shl  "; break;
			}
			core->console.format("    %A  %s %R, %d\n", mip, opstr, r, imm8);
		}
		#endif /* AVMPLUS_VERBOSE */

		REX(Unknown, r, false);

		*mip++ = 0xc1;
		MODRM((Register)op, r);
		*mip++ = (MDInstruction)imm8;
	}

	void CodegenMIR::ALU64(int op, Register r, sintptr disp, Register base)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch (op)
			{
			case 0x85: core->console.format("    %A  test  %d(%R), %R\n", mip, disp, base, r); break;
			case 0x8d: core->console.format("    %A  lea   %R, %d(%R)\n", mip, r, disp, base); break;
			case 0x89: core->console.format("    %A  mov   %d(%R), %R\n", mip, disp, base, r); break;
			case 0x8b: core->console.format("    %A  mov   %R, %d(%R)\n", mip, r, disp, base); break;
			case 0x63: core->console.format("    %A  movsxd   %R, %d(%R)\n", mip, r, disp, base); break;
			case 0xff: 
				switch(r) {
				case 2: core->console.format("    %A  call  %d(%R)\n", mip, disp, base); break;
				case 4: core->console.format("    %A  jmp   %d(%R)\n", mip, disp, base); break;
				case 6: core->console.format("    %A  push  %d(%R)\n", mip, disp, base); break;
				}
			}
		}
		#endif /* AVMPLUS_VERBOSE */

		REX(r, base, true);

		*mip++ = (MDInstruction)op;
		MODRM(r, disp, base);
	}

	void CodegenMIR::ALU(int op, Register r, sintptr disp, Register base)
		{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			switch (op)
		{
			case 0x85: core->console.format("    %A  test  %d(%R), %R\n", mip, disp, base, r); break;
			case 0x8d: core->console.format("    %A  lea   %R, %d(%R)\n", mip, r, disp, base); break;
			case 0x89: core->console.format("    %A  mov   %d(%R), %R\n", mip, disp, base, r); break;
			case 0x8b: core->console.format("    %A  mov   %R, %d(%R)\n", mip, r, disp, base); break;
			case 0xff: 
				switch(r) {
				case 2: core->console.format("    %A  call  %d(%R)\n", mip, disp, base); break;
				case 4: core->console.format("    %A  jmp   %d(%R)\n", mip, disp, base); break;
				case 6: core->console.format("    %A  push  %d(%R)\n", mip, disp, base); break;
		}
			}
		}
		#endif /* AVMPLUS_VERBOSE */

			REX(r, base, false);

		*mip++ = (MDInstruction)op;
		MODRM(r, disp, base);
	}

	void CodegenMIR::JCC(byte op, sintptr offset)
 	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			const char *opstr="?";
			switch(op) {
			case 0x02: opstr = "jb   "; break;
			case 0x03: opstr = "jnb  "; break;
			case 0x04: opstr = "je   "; break;
			case 0x05: opstr = "jne  "; break;
			case 0x06: opstr = "jbe  "; break;
			case 0x07: opstr = "jnbe "; break;
			case 0x0a: opstr = "jp   "; break;
			case 0x0b: opstr = "jnp  "; break;
			case 0x0c: opstr = "jl   "; break;
			case 0x0d: opstr = "jnl  "; break;
			case 0x0e: opstr = "jle  "; break;
			case 0x0f: opstr = "jnle "; break;
			}
			core->console.format("    %A  %s %d\n", mip, opstr, offset);
		}
		#endif /* AVMPLUS_VERBOSE */

		// j<op> off32
		if (is8bit((int)offset)) {
			mip[0] = (MDInstruction)(0x70 | op);
			mip[1] = (MDInstruction)offset;
			mip += 2;
		} else if (is32bit(offset)) {
 			mip[0] = 0x0f;
 			mip[1] = 0x80 | op;
			mip+=2;
 			IMM32((int32)offset);
		}
		else
		{
			AvmAssert(0);
		}
 	}
	
	void CodegenMIR::JMP(sintptr offset)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  jmp   %d\n", mip, offset);
		#endif /* AVMPLUS_VERBOSE */

		if (is8bit((int)offset)) {
			mip[0] = 0xeb;
			mip[1] = (MDInstruction)offset;
			mip += 2;
		} else if (is32bit(offset)) {
 			*mip++ = 0xe9;
 			IMM32((int32)offset);
		}
		else
		{
			AvmAssert(0);
		}
	}

	void CodegenMIR::CALL(sintptr offset)
	{		
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  call  %A\n", mip, offset+(uintptr)(mip+5));
		#endif /* AVMPLUS_VERBOSE */

		if (is32bit(offset))
		{		
			*mip++ = 0xE8;
			IMM32((int32)offset);
		}
		else
		{
			AvmAssert(0);
			*mip++ = 0x4D; //64-bit mode
			*mip++ = 0xFF;
			*mip++ = 0x13; // call address in [R11]
		}
	}

	void CodegenMIR::FPU(int op, Register r)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			const char *opstr="?";
			switch(op) {
			case 0xddd8: opstr = "fstp "; x87Top++; break;
			case 0xddc0: opstr = "ffree"; x87Top++; break;
			case 0xd8c0: opstr = "faddq"; x87Top++; break;
			case 0xd8e0: opstr = "fsubq"; x87Top++; break;
			case 0xd8c8: opstr = "fmulq"; x87Top++; break;
			case 0xd8f0: opstr = "fdivq"; x87Top++; break;
			}
			core->console.format("    %A  %s %X\n", mip, opstr, r);
		}
		#endif /* AVMPLUS_VERBOSE */

		*mip++ = (MDInstruction)(op>>8);
		*mip++ = (MDInstruction)(op&255 | r);
	}

	void CodegenMIR::FPU(int op, sintptr disp, Register base)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			const char *opstr="?";
			switch(op) {
			case 0xdc02: opstr = "fcom "; break;
			case 0xdd03: opstr = "fstpq"; x87Top++; break;
			case 0xdd02: opstr = "fstq "; break;
			case 0xdd00: opstr = "fldq "; x87Top++; break;
			case 0xdf05: opstr = "fildq"; x87Top--; break;
			case 0xdc00: opstr = "faddq"; break;
			case 0xdc04: opstr = "fsubq"; break;
			case 0xdc01: opstr = "fmulq"; break;
			case 0xdc06: opstr = "fdivq"; break;
			case 0xdb00: opstr = "fild "; x87Top--; break;
			}
			core->console.format("    %A  %s %d(%R)\n", mip, opstr, disp, base);
		}
		#endif /* AVMPLUS_VERBOSE */

		AvmAssert(x87Dirty);
		*mip++ = (MDInstruction)(op>>8);
		MODRM((Register)(op&0xff), disp, base);
	}

	void CodegenMIR::FPU(int op)
	{
		incInstructionCount();
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			const char *opstr="?";
			switch(op) {
			case 0xdde9: opstr = "fucomp"; x87Top++; break;
			case 0xd9e0: opstr = "fchs "; break;
			case 0xdfe0: opstr = "fnstsw ax"; break;
			case 0x0f77: opstr = "emms "; x87Top=0; break;
			}
			core->console.format("    %A  %s\n", mip, opstr);
		}
		#endif /* AVMPLUS_VERBOSE */

		mip[0] = (MDInstruction)(op>>8);
		mip[1] = (MDInstruction)(op&255);
		mip += 2;
	}

	void CodegenMIR::TEST_AH(uint8 imm8)
	{
		incInstructionCount();
        #ifdef AVMPLUS_VERBOSE
		if (verbose())
			core->console.format("    %A  test  ah, %d\n", mip, imm8);
        #endif // AVMPLUS_VERBOSE

		mip[0] = 0xf6;
		mip[1] = 0xc4;
		mip[2] = imm8;
		mip += 3;
	}

	bool CodegenMIR::canImmFold(OP *ins, OP *imm) const
	{
		bool can = false;
		if (imm->code==MIR_imm && is32bit(imm->imm))
		{
			switch (ins->code)
			{
			case MIR_lsh:
			case MIR_rsh:
			case MIR_ush:
			case MIR_and:
			case MIR_or:
			case MIR_xor:
			case MIR_add:
			case MIR_sub:
		    case MIR_imul:
			case MIR_icmp:
			case MIR_ucmp:
			case MIR_i2d:
			case MIR_st:
				// can be any 32-bit immediate
				can = true;
				break;
			}
		}
		return can;
	}

	/**
	 * For stacks that are bigger than one page we need to touch each page 
	 * since windows expects that the stack grows at most one page at time
	 * This code was *transcribed* from chkstk.asm
	 */
	void CodegenMIR::emitAllocaProbe(int growthAmt, MdLabel* returnTo)
	{
		AvmAssertMsg (true, "needs fixing for 64-bit");
		#define _PAGESIZE_ 4096

		MDInstruction* label_loop;

			MOV(R10, growthAmt);
	        MOV(R11, RSP);			//      ; current stack pointer in ecx

		label_loop = mip;
			SUB(R11,_PAGESIZE_);    //      ; move down a page
			SUB(R10,_PAGESIZE_);	//		; adjust request and...

			TEST(0, R11, R10);		//      ; ...probe it

			CMP(R10,_PAGESIZE_);	//      ; more than one page requested?
			JNB(1);					//      ; no
			mip[-1] = (MDInstruction)(label_loop-mip);

			SUB(R11,R10);			//      ; move stack down by RAX
			MOV(R10,RSP);			//      ; save current tos and do a...
			TEST(0, R11, R10);		//      ; ...probe in case a page was crossed
			MOV(RSP,R11);			//      ; set the new stack pointer
			JMP(0x7FFFFFFF);		//		; jmp back into mainline code
			mdPatchPrevious(returnTo);

		#undef _PAGESIZE_ 
	}
	
	/**
	 * emitNativeThunk generates code for a native method
	 * thunk.  A native method thunk converts VM types like
	 * Atoms to native data types, pushes them on the stack
	 * in C calling convention, and calls the native method.
	 * By generating code for these, we avoid unnecessary
	 * looping.
	 *
	 * kScratchBufferSize:
	 * To keep our memory footprint small, we want an exact
	 * fit for our native thunk buffers.  We also don't
	 * want to add a lot of code to the VM, and we don't want
	 * two code paths that have to be maintained in
	 * delicate concert.  So, we run the emit algorithm
	 * twice, first to figure out how big the buffer is,
	 * and second to write it out.
	 *
	 * This scratch buffer is used when calculating the
	 * size.  All native methods are baked into the Player,
	 * so the hardcoded buffer size should be OK.
	 * If the assert at the end ever fires, you must enlarge
	 * this buffer.
	 */
	 
	 /*

	 Notes from the 64-bit ABI AMD64, System V documentation:
	 
	 Parameter passing conventions:
	 
	 AMD64 ABI: Integer parameters go into rdi, rsi, rdx, rcx, r8 and r9
	 SSE parameters go into xmm0 through xmm7
	 SSEUP ? 
	 X87 ? 
	 
	 // This routine is called with "MethodEnv* env, int argc, uint32 *ap"
	 
	 env = rdi
	 argc = esi (signed extended to rsi?)
	 rdx = ap

	 In the Windows x64 ABI (yes, it's different), parameters go into
	 rcx, rdx, r8, r9
	ON WINx64,:
	env = rcx
	argc = rdx
	ap = r8

	// The arguments passed in can be native types or Atom types.  They are all 8
	// byte aligned (see unboxCoerceArgs) but types are determined by the traits
	// of the function call.  
	
	// ??? : When pushing arguments on the stack, the called function will 
	// probably expect 32-bit sized args to be only 4 byte pushes???	 	 
	
	*/

	void CodegenMIR::emitNativeThunk(NativeMethod *info)
	{		
		#ifdef AVMPLUS_VERBOSE
		pool->verbose = false;
		#endif // AVMPLUS_VERBOSE

		code = mip = mipStart = getMDBuffer(pool);
		if (!code)
		{
			overflow = true;
			return;
		}

#ifdef FEATURE_BUFFER_GUARD
		GrowthGuard guard(pool->codeBuffer);
#else
		if (!ensureMDBufferCapacity(pool, md_native_thunk_size))
			return;
#endif /* FEATURE_BUFFER_GUARD */

		bool need_rest = ((info->flags & AbstractFunction::NEED_REST) != 0);

#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			core->console << "native thunk for " << info << "\n";
		}
#endif

		// Parameters get passed in RXX regs, or in XMM# regs, depending
		// on type. HOWEVER, they are really viewed as just 4 arguments registers,
		// so if the first integer arg goes in RCX, then the second arg would either go
		// in RDX or XMM1 (NOT XMM0). Crazy.
# ifdef AVMPLUS_WIN32
		const int INT_REGCOUNT = 4;
		const Register intRegUsage[] = {RCX, RDX, R8, R9};
		const int FLOAT_REGCOUNT = 4;
		const Register floatRegUsage[] = {XMM0, XMM1, XMM2, XMM3};
# else
		const int INT_REGCOUNT = 6;
		const Register intRegUsage[] = {RDI, RSI, RDX, RCX, R8, R9};
		const int FLOAT_REGCOUNT = 8;
		const Register floatRegUsage[] = {XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7};
# endif

        int intCount = 0;
        int floatCount = 0;
		//int stackAdjust = 0;

#ifdef DEBUGGER
		// There is already space on the stack for these
		MOV(8, RSP, intRegUsage[0]);	// env
		MOV(16, RSP, intRegUsage[1]);	// argc
		MOV(24, RSP, intRegUsage[2]);	// ap

		// Make room for CallStackNode
		// We stick 8 on the end to 16-byte align the stack, which is
		// unaligned when we enter the function
		const int stack_adjust = BIT_ROUND_UP(sizeof(CallStackNode), 16) + 8;

		// Add room for (4+4) params
		const int param_space = 64;

		int frame_size = stack_adjust + param_space;
 		
        //		SUB(RSP,  frame_size); // make room for callstack
        //		byte *patch_frame_size = mip - 1;

		//debugEnter: 
		// RCX: env (same as func entry)
		// RDX: int argc, (same as func entry)
		// R8: uint32 *ap, (same as func entry)
		// R9: Traits**frameTraits, 
		// stack: int localCount, 
		// stack: CallStackNode* callstack, 
		// stack: Atom* framep, 
		// stack: volatile int *eip

		XOR(R9, R9);			// Traits**
		MOV(32, RSP, R9);		// localCount (0)

		LEA(R10, 64, RSP); // callstack location
		MOV(40, RSP, R10);

		MOV(48, RSP, R9); // framep - R8 is just a convenient zero value
		MOV(56, RSP, R9); // eip - R8 is just a convenient zero value

		thincall(ENVADDR(MethodEnv::debugEnter));

		// reload AP, ARGC.   Leave ENV on stack
		MOV(R8, frame_size+24, RSP);	// AP
		MOV(RDX, frame_size+16, RSP);	// ARGC

		//stackAdjust = 8; // ENV is still on stack

#else
		// Make room for first 4 params, patch later if needed
		const int param_space = 32;
		int stack_adjust = 8;
		int frame_size = stack_adjust + param_space;

        //		SUB(RSP,  frame_size); // make room for callstack
        //		byte *patch_frame_size = mip - 1;

#endif // DEBUGGER

		// Win64 ABI requires 16-byte stack alignment.
		// This SUB will be patched with any needed padding.
		//byte *patch_RSP_padding = NULL;
		//SUB (RSP, 8);
		//patch_RSP_padding = mip-1;
				
		// rax, r11, r10 are scratch registers
		// !!@ emit these only when needed?
		MOV (R10, intRegUsage[2]); // AP
		MOV (RAX, intRegUsage[1]); // ARGC

		// place our 'this' pointer in the first reg slot (RCX)
		MOV (intRegUsage[intCount++], 0, intRegUsage[2]);
		if (info->flags & AbstractFunction::UNBOX_THIS)
		{
			//AvmAssert(0); // needs testing

			// void AND(Register reg, sintptr imm) { ALU(0x25, reg, imm); }
			MOV(R11, ~7);
			ALU64(0x23, intRegUsage[0], R11); // clear atom tag
		}

		// Used for something like Date.getDate
		if (info->flags & AbstractFunction::NATIVE_COOKIE)
		{
			MOV (intRegUsage[intCount++], info->m_cookie);
		}
		
		// In the Win64 ABI, there needs to be a stack slot allocated for
		// ALL parameters, including the first 4 (RCX,RDX,R8,R9). So we need
		// to make space for every parameter

		int push_count = 8;	
		int first_optional = 1 + info->param_count - info->optional_count;
		int arg_offset = 8; // skip our first arg

		for (int i=1; i <= info->param_count; i++)
		{
			// param 0 is receiver, real params start at 1
			Traits* type = info->paramTraits(i);
			AvmAssert(type != VOID_TYPE);
		
			// the arguments have already been coerced to the right type
			// by the caller.  We do not do type conversions here.
			byte* patch_jmp = NULL;

			if (i >= first_optional) 
			{
				// emit code to check whether argument
				// was specified
				CMP (RAX, i);
				JNB (1);
				byte* patch_jae = mip;
				
				// emit code to push the default value
				Atom arg = info->getDefaultValue(i-first_optional);

				if (!type || type == OBJECT_TYPE)
				{
					if (intCount < INT_REGCOUNT)
					{
						MOV (intRegUsage[intCount], arg);
					}
					else
					{
                        AvmAssert(0); // TODO ZELL TODO
						MOV (push_count, RSP, arg); 
					}				
				}
				else if (type == BOOLEAN_TYPE)
				{
					// push bool
					int b = (int)arg>>3;
					if (intCount < INT_REGCOUNT)
					{
						MOV (intRegUsage[intCount], b);
					}
					else
					{
                        AvmAssert(0); // TODO ZELL TODO
						MOV (push_count, RSP, b);
					}	
				}
				else if (type == INT_TYPE)
				{
					int v = (int)AvmCore::integer_i(arg);
					if (intCount < INT_REGCOUNT)
					{
						MOV (intRegUsage[intCount], v);
					}
					else
					{
                        AvmAssert(0); // TODO ZELL TODO
						MOV (push_count, RSP, v);
					}	
				}
				else if (type == UINT_TYPE)
				{
					uint32 v = AvmCore::integer_u(arg);
					if (intCount < INT_REGCOUNT)
					{
						MOV (intRegUsage[intCount], (int32)v);
					}
					else
					{
                        AvmAssert(0); // TODO ZELL TODO
						MOV (push_count, RSP, (int32)v);
					}	

				} 
				else if (type == NUMBER_TYPE)
				{
					// push double
					// TODO make this faster, we probably have memory stalls
					// IA32 calling conventions don't require double's to be 8-aligned,
					// but performance is better if they are.
					double d = AvmCore::number_d(arg);
					int64 dp = *(int64*)&d;
					if (floatCount < FLOAT_REGCOUNT)
					{
						// !!@ better way to do this?
						MOV (R11, dp);
						MOV (-8, RSP, R11);
						MOVSD (floatRegUsage[floatCount], -8, RSP);
					}
					else
					{
                        AvmAssert(0); // TODO ZELL TODO
						MOV (R11, dp);
						MOV (push_count, RSP, R11);
					}
				}
				else
				{
					// push pointer type
					// this case includes kStringType, kObjectType, and kNamRSPaceType
					// default could be null, but won't be undefined
					AvmAssert(arg != undefinedAtom);
					uint64 p = arg & ~7;
					if (intCount < INT_REGCOUNT)
					{
						MOV (intRegUsage[intCount], p);
					}
					else
					{
                        AvmAssert(0); // TODO ZELL TODO
						MOV (push_count, RSP, p);
					}	
				}

				// Insert a JMP instruction here to skip to
				// the next argument
				JMP (1);
				patch_jmp = mip;

				// Patch the JAE instruction to jump here,
				// which is where the non-optional code will
				// go.
				AvmAssert((mip-patch_jae)<256);
				patch_jae[-1] = (byte)(mip-patch_jae);
			}	

			if (type == NUMBER_TYPE)
			{
				// Put float value into register
				if (floatCount < FLOAT_REGCOUNT)
				{
					MOVSD (floatRegUsage[floatCount++], arg_offset, R10);
# ifdef AVMPLUS_WIN32
                    intCount ++;
# endif
				}
				else
				{
                    AvmAssert(0); // TODO ZELL TODO
					MOV (R11, arg_offset, R10);
					MOV (push_count, RSP, R11);
					push_count += 8;
				}
			}
			else
			{
				// Put int/ptr value into register
				if (intCount < INT_REGCOUNT)
				{
					MOV (intRegUsage[intCount++], arg_offset, R10);
# ifdef AVMPLUS_WIN32
                    floatCount ++;
# endif
				}
				else
				{
                    AvmAssert(0); // TODO ZELL TODO
					MOV (R11, arg_offset, R10);
					MOV (push_count, RSP, R11);
					push_count += 8;
				}
			}

			arg_offset += 8;
			
			// Patch the JMP instruction, if applicable,
			// to jump to here.
			if (patch_jmp) {
				patch_jmp[-1] = (byte)(mip-patch_jmp);
			}	
		} // for each param
					
					
					
		// argv, argc need to be passed in either register or stack location
		// depending on registers left over
		if (need_rest)
		{
          AvmAssert(0); // ZELL TODO ZELL
			// rest_count = argc-param_count
			CMP (RAX, info->param_count);

			// rest_count could be <0 if optionals omitted
			JNB (1);
			byte* patch_ip = mip;

			// rest_count<0, push argc=0, argv=NULL
			if (intCount < INT_REGCOUNT)
			{
				MOV(intRegUsage[intCount], 0);
			}
			else
			{
				MOV (push_count, RSP, 0);
			}
			if ((intCount + 1) < INT_REGCOUNT)
			{
				MOV(intRegUsage[intCount + 1], 0);
			}
			else
			{
				MOV (push_count + 8, RSP, 0);
			}	
				
			JMP (1);

			patch_ip[-1] = (byte)(mip-patch_ip);
			patch_ip = mip;
			if (intCount < INT_REGCOUNT)
			{
				LEA(intRegUsage[intCount++], arg_offset, R10); // callstack location
				push_count += 8;
			}
			else
			{
				LEA(R10, arg_offset, R10); // callstack location
				MOV (push_count, RSP, R10);
				push_count += 8;
			}	

			SUB (RAX, info->param_count);
			if (intCount < INT_REGCOUNT)
			{
				MOV (intRegUsage[intCount++], RAX);
				push_count += 8;
			}
			else
			{
				MOV (push_count, RSP, RAX);
				push_count += 8;
			}	
			
			patch_ip[-1] = (byte)(mip-patch_ip);
		}

		// all args have been pushed, now call function using thiscall calling conventions
		Traits* type = info->returnTraits();

		byte* next_ip = mip+4;  // branch rel. to next instr
		
		// !!@ can our offset ever be > 32 bit
		size_t offset = info->m_handler_addr - (uintptr)next_ip - 1;
		if (is32bit(offset))
			CALL (info->m_handler_addr - (uintptr)next_ip - 1);       // call the method as an instance method
		else
		{
			MOV(R11, (sintptr)&info->m_handler_addr);
			CALL(0, R11);
		}

		// need to patch the frame size (maybe) depending
		// on the parameter count
		//byte cur_frame_size = *patch_frame_size;
		if (push_count>frame_size)
		{
			frame_size = stack_adjust + push_count;
			if (!(frame_size & 0x8))
				frame_size += 8;

            //			*patch_frame_size = (byte)frame_size;
            //			core->console << "Frame size patched from: " << cur_frame_size <<
            //			  " to "<< frame_size << "\n";
		}


// debugExit logic
#ifdef DEBUGGER

	// rdi - get ENV back off the stack
	MOV(intRegUsage[0], frame_size+8, RSP);  
	// rdx - callstack pointer
	LEA(intRegUsage[1], param_space, RSP); // callstack location

	// store the return value on the stack if not a double.	
	if (type != NUMBER_TYPE)
	{
		// Just use a spot on the stack
		MOV(40, RSP, RAX);
		//SUB (RSP, 8); // 16-byte stack alignment required
		//PUSH (RAX);
	}

	// call debugExit
	thincall(ENVADDR(MethodEnv::debugExit));
	
	if (type != NUMBER_TYPE)
	{
		MOV(RAX, 40, RSP);
		//POP (RAX);
		// patch up our stack - get rid of callstack allocation	
		//ADD(RSP, stack_adjust + 8);
	}
	else
	{
		// patch up our stack - get rid of callstack allocation	
		//ADD(RSP, stack_adjust);
	}

#endif

		if (type != NUMBER_TYPE)
		{
			// result in RAX
			
			if (type == BOOLEAN_TYPE)
			{
				// return value already in RAX
				// in VC++ bool is just a byte, so mask it off
# ifdef AVMPLUS_WIN32
				MOVZX_r8 (RAX, RAX);
# endif
			}
			else if (type == INT_TYPE)
			{
				// sign extend EAX to RAX
				REX (RAX, RAX, true);
 				*mip++ = (MDInstruction)(0x98); // CDQE instruction
			}

			else if (type == VOID_TYPE)
			{
				MOV (RAX, undefinedAtom);
			}
		}
		// else, result in SSE register XMM0??
	
        //		ADD(RSP, frame_size);
		
		RET  ();

		bindMethod(info);
		
		//pool->verbose = false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	void* CodegenMIR::emitImtThunk(ImtBuilder::ImtEntry *e)
	{
		//AvmAssert(0); // needs to be ported to 64-bit
		mip = mipStart = getMDBuffer(pool);

#ifdef FEATURE_BUFFER_GUARD
		GrowthGuard guard(pool->codeBuffer);
#else
		if (!ensureMDBufferCapacity(pool, md_native_thunk_size))
			return NULL;
#endif /* FEATURE_BUFFER_GUARD */

		// the generated thunk does not call any helper methods, so we are
		// free to use rcx, rdx, r8, r9 as scratch regs without touchning the
		// stack.

		// in:	RCX = MethodEnv
		//		RDX = argc
		//		R8 = ap
		//		R9 = iid
		//		0(ap) = ScriptObject (concrete instance of class)
		

		// Just use R10-R11, since we don't want to mess with other things

#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			core->console << "imt thunk\n";
		}
#endif

		// ap currently in R8
		MOV (R10, 0, R8);			// obj
		MOV (R10, offsetof(ScriptObject, vtable), R10); // vtable

		AvmAssert(e->next != NULL); // must have 2 or more entries

		while (e->next)
		{
			ImtBuilder::ImtEntry *next = e->next;

			#ifdef AVMPLUS_VERBOSE
			if (verbose())
			{
				core->console << "              disp_id="<< e->disp_id << " " << e->virt << "\n";
			}
			#endif

			// Compare iid with the passed in iid
			CMP (R9, e->virt->iid());
			JNE (1);

			byte* patchip = mip;
			// Load vmethod addr into RAX
			MOV (RCX, offsetof(VTable,methods)+8*e->disp_id, R10); // load concrete env
			//MOV (_env, RSP, RAX);  // replace env before call
			JMP (offsetof(MethodEnv, impl32), RCX); // invoke real method indirectly
			patchip[-1] = (byte)(mip-patchip);

			pool->core->GetGC()->Free(e);
			e = next;
		}

		// last one is unconditional
		#ifdef AVMPLUS_VERBOSE
		if (verbose())
		{
			core->console << "              disp_id="<< e->disp_id << " " << e->virt << "\n";
		}
		#endif
		MOV (RCX, offsetof(VTable,methods)+8*e->disp_id, R10); // load concrete env
		//MOV (_env, RSP, RAX);  // replace env before call
		JMP (offsetof(MethodEnv, impl32), RCX); // invoke real method indirectly

#ifdef AVMPLUS_JIT_READONLY
		makeCodeExecutable();
#endif /* AVMPLUS_JIT_READONLY */
		
		// lock in the next available location in the buffer (16B aligned)
		pool->codeBuffer->setPos((byte*) ( (size_t)mip+15 & ~15 ));

		return mipStart;
	}

#endif /* AVMPLUS_AMD64 */

}
