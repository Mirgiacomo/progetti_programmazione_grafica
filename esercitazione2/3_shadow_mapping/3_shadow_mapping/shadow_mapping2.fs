#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

// input la posizione del frammento di spazio luminoso e la mappa di profondità generata dal primo passaggio di rendering
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // trasformare la posizione del frammento di spazio luminoso nel clip-space in coordinate del dispositivo normalizzate.
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // trasformiamo le coordinate nell'intervallo [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // prendo z del vettore proiettato che è uguale alla profondità di questo frammento dalla prospettiva della luce
    float currentDepth = projCoords.z;
    // calcolo il bias
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    // il valore della distorsione dipenda fortemente dall'angolo tra la sorgente di luce e la superficie.
    // Qui abbiamo un bias massimo di 0,05 e un minimo di 0,005 in base alla direzione normale e della luce della superficie. 
    // le superfici come il pavimento che sono quasi perpendicolari alla sorgente luminosa ottengono una piccola distorsione, 
    // mentre le superfici come le facce laterali del cubo ottengono una distorsione molto maggiore
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF: campionare i texel circostanti della mappa di profondità e calcolare la media dei risultati:
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // forziamo il valore dell'ombra su 0,0 ogni volta che la coordinata z del vettore proiettato è maggiore di 1,0
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    // restituisce la posizione dello spazio luminoso del frammento nell'intervallo [-1,1]
    return shadow;
}

void main()
{       
    // implementa il lighting e colore
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calcolo la shadow (illuminazione Blinn-Phong)
    // Le componenti diffuse e speculari risultanti vengono quindi moltiplicate per questa componente ombra
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}