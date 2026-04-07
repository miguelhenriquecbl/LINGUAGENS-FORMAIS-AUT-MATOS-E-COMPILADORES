{ Teste 001 - Exemplo do professor com adicao de numeros com sinal e fator de escala }
program Exemplo;
var
    x, y : integer;
    z    : real;
begin
    x := +1.0;
    y := -20;
    z := 1.5E+2;
    if x > y then
        x := x - 1
    else
        y := y + 1;
    while z <= 100 do
    begin
        z := z * 1.5E-1;
        x := x + 2
    end
end.



