#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // moltiplichiamo il colore della luce per un piccolo fattore ambientale costante e lo moltiplichiamo per il colore dell'oggetto 
    // lo usiamo come colore del frammento nello shader dell'oggetto cubo
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // il vettore di direzione della luce è il vettore di differenza tra il vettore di posizione della luce e il vettore di posizione del frammento
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // Calcoliamo l'impatto diffuso della luce sul frammento corrente (diffusa)
    // Nota: se l'angolo è > 90 gradi  il risultato sarà negativo, usando il max ci assicuriamo che prenda il valore più alto dei parametri
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // moltiplico per ottenere il colore di output del frammento risultante        
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
} 