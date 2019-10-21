import nltk
import pyphen
from nltk.probability import *
import nltk.corpus
import re
import string

dic = pyphen.Pyphen(lang='pt-BR')
escrita=[]
banco = open("nuBank.txt", 'w', encoding='utf-8')
corpus = nltk.corpus.floresta
corpus.ensure_loaded()
freq = nltk.FreqDist(corpus.words()) 
for palavra, frequencia in freq.most_common(30000):
    palavra = re.sub('['+string.punctuation+']', '', palavra)
    if not palavra.isnumeric() and palavra is not "" and "truiu" not in palavra and palavra is not "art":
        escrita.append(palavra)
        escrita.append("\n")
for palavra in escrita:
    palavra_hyphen = dic.inserted(palavra)
    banco.write(palavra_hyphen)
banco.close

