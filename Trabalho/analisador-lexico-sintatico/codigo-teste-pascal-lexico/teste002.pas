{ Teste 002 - Operadores relacionais com numeros com sinal }
program Operadores;
var
    a, b, c : integer;
begin
    a := +100;
    b := -200;
    c := +0;
    if a <> b then
        c := +1;
    if a < b then
        c := -1;
    if a <= b then
        c := c + +1;
    if b > a then
        c := c - -1;
    if b >= a then
        c := c + +10;
    if a = c then
        c := +0
end.




