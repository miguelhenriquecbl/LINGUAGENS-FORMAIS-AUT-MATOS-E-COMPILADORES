{ Teste 003 - Programa correto com comentarios }
{ Este programa testa se os comentarios sao ignorados corretamente }
program Comentarios;
var
    x : integer;
    y : real;
begin
    { atribuindo valores iniciais }
    x := 42;
    y := 3.14;
    { verificando condicao }
    if x > 10 then
        x := x - 1  { decrementa x }
    else
        x := x + 1; { incrementa x }
    while x > 0 do
    begin
        x := x - 1  { loop ate zero }
    end
end.
