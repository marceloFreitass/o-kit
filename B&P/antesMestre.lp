\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 1000000 λ0 + 1000000 λ1 + 1000000 λ2 + 1000000 λ3
       + 1000000 λ4 + λ5 + λ6 + λ7 + λ8 + λ9
Subject To
 c1: λ0 + λ5 + λ7  = 1
 c2: λ1 + λ5 + λ6  = 1
 c3: λ2 + λ6 + λ7  = 1
 c4: λ3 + λ8  = 1
 c5: λ4 + λ9  = 1
End
