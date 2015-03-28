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
    float4 color : COLOR0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
    PixelInputType output; 
    
	float4 pos = float4(input.position);
    // Change the position vector to be 4 units for proper matrix calculations.
   // input.position.w = 1.0f;
	//input.position.z = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    pos = mul(pos, modelView); 
	pos = mul(pos, projection);
	output.position = pos;

    /* output.position.x = 10;
    output.position.y = 10;
    output.position.z = 0.0;
    output.position.w = 0.0;

    output.position.x += 10 * random(float2(output.position.x, output.position.y));
    output.position.y += 10 * random(float2(output.position.x, output.position.y)); */
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
}




