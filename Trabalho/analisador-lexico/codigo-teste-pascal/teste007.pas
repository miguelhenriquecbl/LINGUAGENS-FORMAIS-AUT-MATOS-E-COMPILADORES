{ Teste 007 - Programa com multiplos erros lexicos }
program MultiploErros;
var
    x : integer;
    y : real;
begin
    x := 10 @ 2;
    y := 3.14 % 2;
    x := x $ y;
    x := x + 1
end.
