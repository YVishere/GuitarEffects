################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/tools/compiler/ti-cgt-c6000_8.3.8/bin/cl6x" -mv6740 --include_path="I:/Flanging Effect" --include_path="C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/tools/compiler/ti-cgt-c6000_8.3.8/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/tools/compiler/ti-cgt-c6000_8.3.8/bin/cl6x" -mv6740 --include_path="I:/Flanging Effect" --include_path="C:/ProgramData/App-V/883E8076-D99D-4911-93C4-940332A6BE1F/BFE39798-EFFD-4CAF-929B-E5A85FC35347/Root/ccs/tools/compiler/ti-cgt-c6000_8.3.8/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


