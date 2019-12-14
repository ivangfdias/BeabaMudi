import subprocess
import time
process = subprocess.Popen([r"cmd"])

def strip_end(text, suffix):
    if not text.endswith(suffix):
        return text
    return text[:len(text)-len(suffix)]

def nome():
    sub = subprocess.Popen([r"./geraNome"])
    arquivo = open("nomeGerado.txt", "r", encoding = "utf-8")
    for line in arquivo:
        novoNome = line
        novoNome = strip_end(novoNome, "\n")
    arquivo.close
    process.terminate()
    return novoNome.title()

def sobrenome(quantidade):
    sub = subprocess.Popen([r"./geraSobrenome"])
    sub.communicate(str(quantidade))
    arquivo = open("sobrenomeGerado.txt", "r", encoding = "utf-8")
    novoNome = ""
    for line in arquivo:
        novoNome = novoNome + strip_end(line, "\n") + " "
    arquivo.close   
    process.terminate()
    novoNome = strip_end(novoNome, " ")
    return novoNome.title()
