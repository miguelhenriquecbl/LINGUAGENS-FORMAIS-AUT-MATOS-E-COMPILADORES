{ Teste 003 - Comentarios com numeros com sinal e fator de escala }
program Comentarios;
var
    x : integer;
    y : real;
begin
    { atribuindo valores com sinal }
    x := +42;
    y := -3.14E+2;
    { verificando condicao }
    if x > +10 then
        x := x - +1  { decrementa x }
    else
        x := x + -1; { incrementa x negativamente }
    while x > -10 do
    begin
        x := x - +1  { loop ate -10 }
    end
end.



