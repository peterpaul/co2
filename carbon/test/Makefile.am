EXTRA_DIST=pass fail generate-testcases.sh run_pass_test.sh run_fail_test.sh testignore testdonotcreate
MAINTAINERCLEANFILES=Makefile.in

targetdir=target

TESTS_ENVIRONMENT=CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LD="$(LD)" SRCDIR="$(srcdir)"

distclean-local:
	-rm -rf $(targetdir)

clean-local:
	-rm -f $(TESTS)
	-cat $(srcdir)/testdonotcreate | xargs rm -f
	-cat $(srcdir)/testignore | xargs rm -f

$(TESTS): $(srcdir)/generate-testcases.sh $(srcdir)/testdonotcreate
	$(srcdir)/generate-testcases.sh $(srcdir)

Makefile.am: $(srcdir)/generate-makefile-am.sh $(srcdir)/testignore
	$(srcdir)/generate-makefile-am.sh

TESTS=\
pass/arithmetic_expressions.sh\
pass/array.sh\
pass/cast_expression.sh\
pass/class_conditional_dot_expr.sh\
pass/class_decl_abstract.sh\
pass/class_decl_constructor_parameters.sh\
pass/class_decl_constructor_subclass_parameters.sh\
pass/class_decl_constructor.sh\
pass/class_decl_constructor_vararg.sh\
pass/class_decl_destructor.sh\
pass/class_decl_get_class.sh\
pass/class_decl_inheritance.sh\
pass/class_decl_names.sh\
pass/class_decl_object_vars.sh\
pass/class_decl_self.sh\
pass/class_decl_super.sh\
pass/class_decl.sh\
pass/class_expression.sh\
pass/class_intergrated.sh\
pass/class_is_of.sh\
pass/class_type_def_order.sh\
pass/dependencies.sh\
pass/expr_conditional_expr.sh\
pass/external_definition.sh\
pass/function_call.sh\
pass/function_definitions.sh\
pass/function_nested_class.sh\
pass/function_nested.sh\
pass/function_types_def.sh\
pass/function_types.sh\
pass/fun_decl.sh\
pass/fun_decl_vararg.sh\
pass/fun_type.sh\
pass/global_scope.sh\
pass/import.sh\
pass/include_subfolder.sh\
pass/include_variable_and_function.sh\
pass/interface_cast.sh\
pass/interface_decl_diamont.sh\
pass/interface_decl_mixin_diamont.sh\
pass/interface_decl_mixin.sh\
pass/interface_decl.sh\
pass/interface_implementation.sh\
pass/interface_multiple.sh\
pass/interface_override.sh\
pass/List.sh\
pass/loops.sh\
pass/Object.sh\
pass/ray.sh\
pass/scope_override_declaration.sh\
pass/scope_override.sh\
pass/scope_simple.sh\
pass/scope.sh\
pass/self.sh\
pass/size_expression.sh\
pass/struct_decl.sh\
pass/switch_case_statement_identifier.sh\
pass/switch_case_statement.sh\
pass/try_statement_finally.sh\
pass/try_statement_nested_finally.sh\
pass/try_statement_nested_return.sh\
pass/try_statement_nested.sh\
pass/try_statement_return_from_finally.sh\
pass/try_statement_return.sh\
pass/try_statement.sh\
pass/try_test.sh\
pass/try_test_throw.sh\
pass/typedef.sh\
pass/va_arg.sh\
pass/var_decl.sh\
fail/class_decl_constructor_arguments.sh\
fail/class_decl_constructor_decl.sh\
fail/class_decl_constructor_name.sh\
fail/class_decl_constructor.sh\
fail/class_decl_redecl_var.sh\
fail/class_decl.sh\
fail/class_decl_type_check.sh\
fail/class_function_type_error.sh\
fail/function_decl_args.sh\
fail/function_decl.sh\
fail/function_decl_vararg.sh\
fail/import.sh\
fail/scope.sh\
fail/self.sh\
fail/type_compat_int_float_1.sh\
fail/type_compat_int_float_2.sh\
fail/type_compat_int_float_3.sh\
fail/type_compat_int_float_4.sh