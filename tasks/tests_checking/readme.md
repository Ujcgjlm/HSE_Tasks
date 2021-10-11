# Проверка работ

В ВШЭ проводится письменная контрольная работа. `N` студентов сдают свои работы в общую стопку, причем некоторые кладут свою работу сверху, а другие (считая, что чем позже их работу проверят, тем лучше) - снизу.

Проверяются работы в том порядке, в котором лежат, начиная с верхней.
Определите, чья работа будет проверена `k`-й по счёту.

В функцию передаются:
 - список из N действий студентов
  - список из `M` чисел `x_i` от `1` до `N` - номер в стопке очередной интересующей нас работы, для которой мы хотим определить автора.
 
 Ожидается, что функция вернет список фамилий: на `i`-том месте в списке должна стоять фамилия студента, работу которого проверят `x_i`-й.
 