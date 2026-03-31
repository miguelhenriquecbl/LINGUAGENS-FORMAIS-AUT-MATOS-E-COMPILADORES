{ Teste 008 - Erro: multiplos erros com novas regras }
program MultiploErros;
var
    x : integer;
    y : real;
begin
    x := +10 @ -2;
    y := 1.5E# + 2.0E$;
    x := x + +1
end.
