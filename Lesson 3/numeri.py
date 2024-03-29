#! /usr/bin/env python3

# funzione che visualizza degli interi dispari con un numero massimo
# di cifre assegnato da linea di comandi
# usate la ridirezione per salvare gli interi in un file in formato testo
 
import random
import sys

def primo(n):
    assert n>0, "L'input deve essere positivo"
    if n==1:
        return False
    if n==2:
        return True
    if n%2 == 0:
        return False
    assert n>=3 and n%2==1, "C'e' qualcosa che non funziona"
    for i in range(3,n//2,2):
        if n%i==0:
            #print ("Ho trovato", i, "che divide", n)
            return False
        if i*i > n:
            break    
    return True


def main(nomefile,cifre):
  f = open(nomefile,"w")
  assert(f!=None)
  tot_primi = 0
  for i in range(10):
    x = random.randint(0, 10**(cifre))
    if x%2==0:
      x += 1
    while x%3==0 or x%5 ==0 or x%7==0:
      x += 2
    if primo(x):
      tot_primi += 1  
    print(x)
  print("Trovati",tot_primi,"primi",file=sys.stderr)
  f.close()

if len(sys.argv)!=3:
  print("Uso:\n\t %s numero_interi cifre" % sys.argv[0])
else:
  main(sys.argv[1], int(sys.argv[2]) )
  
  
      
