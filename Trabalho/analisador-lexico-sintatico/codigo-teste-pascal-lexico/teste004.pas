{ Teste 004 - Case-insensitive com fator de escala E e e }
PROGRAM CaseInsensitive;
VAR
    Contador : INTEGER;
    Media    : REAL;
BEGIN
    Contador := +0;
    Media := 1.5e+2;
    WHILE Contador <= +10 DO
    BEGIN
        Contador := Contador + +1;
        Media := Media + 1.0E-1
    END;
    IF Media > +5 THEN
        Contador := Contador - +1
    ELSE
        Contador := Contador + -1
END.



