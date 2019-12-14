import nltk
import pyphen
from nltk.probability import *
import nltk.corpus
import re
import string
def hasNumbers(inputString):
    return any(char.isdigit() for char in inputString)

dic = pyphen.Pyphen(lang='pt-BR')
escrita=[]
banco = open("nuBank.txt", 'w', encoding='utf-8')
corpus = nltk.corpus.machado
corpus.ensure_loaded()
freq = nltk.FreqDist(corpus.words()) 
for palavra, frequencia in freq.most_common(42000):
    palavra = re.sub('['+string.punctuation+']', '', palavra)
    if palavra is not "" and "truiu" not in palavra and palavra is not "art" and not hasNumbers(palavra) and palavra not in escrita:
        escrita.append(palavra)
        escrita.append(".\n")
for palavra in escrita:
    palavra_hyphen = dic.inserted(palavra)
    banco.write(palavra_hyphen)
banco.close

