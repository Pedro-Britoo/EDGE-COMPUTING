# OrbitGuard-Edge (Versão Arduino Serial)

**Estação IoT de detecção precoce de queimadas**
Entregável da disciplina **Edge Computing & Computer Systems** · **FIAP** · **GS 2026** · **Indústria Espacial**

---

## 📖 Descrição do Projeto

Firmware para Arduino que realiza a leitura de temperatura, concentração de fumaça e luminosidade ambiente, calcula um **score de risco de incêndio em vegetação** e envia o resultado em formato **JSON via Monitor Serial** para processamento por um gateway ou backend OrbitGuard.

A solução integra uma plataforma maior que cruza dados coletados em borda (*edge computing*) com informações satelitais abertas provenientes de:

* NASA FIRMS
* INPE Queimadas
* Copernicus Sentinel-2

---

## 🎯 Objetivo da Solução

Reduzir o tempo entre a ignição de um foco de incêndio e o despacho da brigada de combate, fornecendo confirmação local em tempo real e monitoramento contínuo das condições ambientais.

---

## 🔧 Componentes Utilizados

* 1× Placa Arduino (Uno, Nano ou compatível)
* 1× Sensor de Temperatura TMP36
* 1× Sensor de Fumaça/Gás MQ-2
* 1× LDR (Sensor de Luminosidade)
* 1× LED Verde
* 1× LED Vermelho
* 1× Buzzer Ativo
* 1× Display LCD I²C 16×2 (endereço `0x27`)
* Protoboard e Jumpers

---

## ⚙️ Funcionamento do Sistema

1. O sistema inicia exibindo uma mensagem de boas-vindas no LCD e acendendo o LED verde.
2. Em intervalos regulares, o firmware realiza a leitura dos sensores:

   * Temperatura (TMP36)
   * Fumaça (MQ-2)
   * Luminosidade (LDR)
3. Um algoritmo de pontuação (*score*) avalia o risco de incêndio utilizando limites pré-definidos.
4. O estado operacional do sistema é atualizado conforme o score obtido.
5. Os dados são exibidos no LCD e enviados em formato JSON pela porta serial.

---

## 📊 Regras de Pontuação

### Temperatura

| Condição | Valor      |
| -------- | ---------- |
| ≥ 55°C   | +50 pontos |
| ≥ 42°C   | +25 pontos |

### Fumaça

| Condição | Valor      |
| -------- | ---------- |
| ≥ 600    | +40 pontos |
| ≥ 300    | +20 pontos |

### Luminosidade (Clarão)

| Condição | Valor      |
| -------- | ---------- |
| ≥ 800    | +15 pontos |

---

## 🚨 Estados do Sistema

### 🟢 NORMAL

**Score < 35**

* Leitura a cada 5 segundos
* LED verde ligado
* Sem alerta sonoro

### 🟡 ATENÇÃO

**Score entre 35 e 69**

* Leitura a cada 2 segundos
* LEDs verde e vermelho ligados
* Bip curto do buzzer

### 🔴 CRÍTICO

**Score ≥ 70**

* Leitura a cada 2 segundos
* Apenas LED vermelho ligado
* Sirene de alerta prolongada

---

## 📺 Informações Exibidas no LCD

O display LCD apresenta:

* Temperatura (T)
* Fumaça (F)
* Status Atual

Exemplo:

```text
T: 43.2C F:350
ATENCAO
```

---

## 📡 Comunicação Serial

A cada ciclo de leitura, o sistema envia uma carga útil JSON pela interface serial.

### Exemplo

```json
{
  "T": 24.50,
  "F": 120,
  "L": 300,
  "Status": "NORMAL"
}
```

### Configuração

```text
Baud Rate: 9600
```

---

## 🔌 Estrutura do Circuito

| Componente   | Pino Arduino | Tipo              | Observação                      |
| ------------ | ------------ | ----------------- | ------------------------------- |
| TMP36        | A0           | Analógico         | Conversão baseada em 5V         |
| MQ-2         | A1           | Analógico         | Saída analógica do sensor       |
| LDR          | A2           | Analógico         | Utilizar divisor de tensão      |
| LED Verde    | 8            | Saída Digital     | Recomenda-se resistor limitador |
| LED Vermelho | 9            | Saída Digital     | Recomenda-se resistor limitador |
| Buzzer Ativo | 10           | Saída Digital/PWM | Acionado por `tone()`           |
| LCD I²C SDA  | A4           | I²C               | Endereço `0x27`                 |
| LCD I²C SCL  | A5           | I²C               | Comunicação do display          |

---

## 🚀 Instruções de Execução

### Arduino IDE

1. Abra o arquivo `.ino`.
2. Instale a biblioteca:

```text
LiquidCrystal_I2C
(Frank de Brabander)
```

3. Selecione a placa:

   * Arduino Uno
   * Arduino Nano

4. Monte o circuito conforme a tabela de pinagem.

5. Compile e carregue o código.

6. Abra o **Monitor Serial**.

7. Configure:

```text
Baud Rate: 9600
```

8. Observe os dados JSON sendo transmitidos.

---

## 💻 Simulação

O projeto pode ser executado em:

https://www.tinkercad.com/things/9Meog7P4z3i/editel?returnTo=%2Fdashboard
---

## 👥 Integrantes do Grupo

| Nome                             | RM     |
| -------------------------------- | ------ |
| Vitor Santos Cintra              | 569416 |
| Pedro Henrique Santos de Brito   | 573187 |
| Octavio Kal Oliveira Lima        | 571838 |
| Douglas Ferreira Santos Patriota | 569713 |

---

link  video https://www.youtube.com/shorts/Rr76OjzVFLM

## 📄 Licença

Projeto acadêmico desenvolvido para a disciplina **Edge Computing & Computer Systems – FIAP (GS 2026)**.
Não destinado para uso operacional em sistemas reais de prevenção ou combate a incêndios sem validação técnica adicional.
