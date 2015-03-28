////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix modelView;
    matrix projection;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float4 color : COLOR0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 color : COLOR0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{ 
    PixelInputType output; 
     
	float4 pos = float4(input.position);

    // Calculate the position of the vertex against the world, view, and projection matrices.
    pos = mul(pos, modelView); 
	pos = mul(pos, projection); 
	output.position = pos;

    // Store texture coordinate for pixel shader.
    output.tex = input.tex;
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
}




