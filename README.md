# Medidor de temperatura (LM35) em rede (arduino e ethernet shield)

A ideia deste projeto consiste num servidor (Arduino + ethernet shield) que
obtém a temperatura do ambiente e se comunica com o cliente que irá estar
em seu notebook. 

Dependendo da temperatura do ambiente enviada do
servidor para o cliente, este poderá fazer uma requisição, momentos antes
de chegar em casa, para que o ar condicionado seja ligado na temperatura
desejada.

Para ilustrar a temperatura do ar condicionado, utilizamos 3 LEDS, sendo
que cada um deles corresponde a uma faixa de valores de temperatura em o
C.

Assim, tanto no momento em que o cliente consulta a temperatura que o ambiente está quanto no momento em que ele estipula a temperatura desejada
para o ar condicionado antes de chegar em casa, um dos três leds ira acender, de acordo com as faixas de temperatura, estabelecidas na tabela I

<!DOCTYPE html>
<html>
<head>
<style>
table {
  font-family: arial, sans-serif;
  border-collapse: collapse;
  width: 100%;
}

td, th {
  border: 1px solid #dddddd;
  text-align: left;
  padding: 8px;
}

tr:nth-child(even) {
  background-color: #dddddd;
}
</style>
</head>
<body>
<table>
  <tr>
    <th>LED</th>
    <th>Faixa de temperatura (t)</th>
  </tr>
  
  <tr>
    <td>LED 1</td>
    <td>t <= 25</td> 
  </tr>
  
  <tr>
    <td>LED 2</td>
    <td>(t> 25) && (t<=27)</td>
  </tr>
  
  <tr>
    <td>LED 3</td>
    <td>(t> 27) && (t<=40)</td>
  </tr> 
</table>

</body>
</html>
Tabela I: LEDs e faixas de temperaturas correspondentes.



