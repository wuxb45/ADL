" in "../filetype.vim", add line:
" au BufNewFile,Bufread *.adl setf adl



if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" let @C == C syntax
syntax include @C syntax/c.vim

syn region behavior matchgroup=ADLc start="%%" end="%%" contains=@C

syn region ADLdatatype matchgroup=ADLxxx start="\[" end="\]" contains=ADLtype  

" comment
syn match ADLcomment /#.*$/
" include
syn match ADLinclude /^@.*$/ 

syn keyword ADLmodule module 
syn keyword ADLkeyword input output inout contain net ram
syn keyword ADLtype u32 u16 u08 i32 i16 i08 f32 f64 w01 contained
syn keyword ADLtype U32 U16 U08 I32 I16 I08 F32 F64 W01 contained

hi def link ADLxxx Identifier 
hi def link ADLcomment comment
hi def link ADLinclude include
hi def link ADLkeyword keyword
hi def link ADLtype type
hi def link ADLmodule keyword
hi def link ADLc special

let b:current_syntax = "adl"
