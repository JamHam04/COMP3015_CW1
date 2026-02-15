# COMP3015_CW1
# Initial Prototype – OpenGL Custom Shading Model

## Development Environment

- **IDE:** Visual Studio 2022
- **Operating System:** Windows 11 (64-bit)
- **OpenGL Version:** 4.6
- **GLSL Version:** 460

---

## Overview

This prototype implements a custom Blinn-Phong shading model in GLSL, combined with:

- Multiple dynamic light sources (3 lights)
- Normal mapping
- Texture blending (damage overlay on floor)
- HDR rendering
- Bloom (Gaussian blur)
- Gamma correction
- Fog
- Skybox rendering

### The scene consists of:

- A textured floor
- Four enclosing walls
- Roof fragments
- A concrete barrier
- A barrel with animated fire lighting
- HDR skybox environment

---

## Shading Model

The lighting model is a custom Blinn-Phong implementation written in GLSL:

```glsl
vec3 blinnPhong(LightInfo light, vec3 position, vec3 normal, vec3 texture);
```

Each light contributes:

- Ambient term
- Diffuse term
- Specular term

Multiplied by a custom intensity parameter:

```glsl
light.L
```

---

## Lighting Setup

Three lights are used:

- **Light 0** – Static scene light
- **Light 1** – Secondary fill light
- **Light 2** – Animated fire light inside the barrel

The fire light intensity flickers over time:

```cpp
float fireIntensity = 1.0f + 0.75f * sin(tPrev * 5.0f);
```

This produces dynamic illumination and makes the scene feel more alive.

---

## Texturing Technique

The project uses multiple texture-based techniques:

### 1. Diffuse + Normal Mapping

Each object supports:

- Diffuse texture
- Normal map

Normal maps are converted from texture space:

```glsl
normalTex = 2.0 * normalTex - 1.0;
```

### 2. Texture Mixing (Damage Overlay)

The floor supports blending between:

- Base asphalt texture
- Damage overlay texture

The blend factor comes from the alpha channel of the damage texture:

```glsl
float mixFactor = mixDiffuseTex.a;
finalDiffuseTex = mix(diffuseTex, mixDiffuseTex.rgb, mixFactor);
```

This allows layered materials and adds visual complexity without extra geometry.

---

## Rendering Pipeline

Rendering is implemented as a 5-pass pipeline:

### Pass 1 – HDR Scene Rendering

- Scene rendered to floating-point framebuffer (`GL_RGB32F`)
- Custom lighting + fog applied
- Output stored in `hdrTexture`

### Pass 2 – Brightness Extraction

- Extracts pixels above luminance threshold (`LumThresh`)
- Prepares bright regions for bloom

### Pass 3 – Vertical Gaussian Blur

- Applies weighted blur using precomputed Gaussian weights

### Pass 4 – Horizontal Gaussian Blur

- Completes separable Gaussian blur

### Pass 5 – Tone Mapping + Bloom Combination

- Applies filmic tone mapping
- Adds bloom texture
- Applies gamma correction (2.2)

Tone mapping uses an XYZ color space conversion and white point compression:

```glsl
lumFactor = (lumFactor * (1.0 + lumFactor / (White * White))) / (1.0 + lumFactor);
```

This prevents bright areas from clipping while preserving contrast.

---

## Fog Implementation

Fog is distance-based:

```glsl
float fogFactor = clamp((Fog.maxDist - distance) / (Fog.maxDist - Fog.minDist), 0.0, 1.0);
```

---

## Skybox

An HDR cubemap is rendered using a separate shader program.

- Gamma correction is applied
- Brightness is reduced slightly to maintain a dark mood

The skybox is rendered first using a modified view matrix:

```glsl
mat4 skyboxView = mat4(mat3(view));
```

---

## Camera System

The camera includes:

- WASD movement
- Mouse look (yaw/pitch)
- Pitch clamping
- Adjustable movement speed

The camera updates the view matrix every frame.

---

## Code Structure and Navigation

### SceneBasic_Uniform.cpp

Main responsibilities:

- Scene initialization (`initScene`)
- Shader compilation (`compile`)
- Framebuffer setup (`setupFBO`)
- Render passes (`pass1–pass5`)
- Scene drawing (`drawScene`)
- Camera input (`userInput`)
- Luminance computation (`computeLogAveLuminance`)

---

## Shader Files

### basic_uniform.vert

- Transforms positions
- Computes normals
- Passes texture coordinates

### basic_uniform.frag

Contains:

- Blinn-Phong lighting
- Texture mixing
- Normal mapping
- Fog
- HDR extraction
- Gaussian blur
- Tone mapping
- Gamma correction

### skybox.vert / skybox.frag

Dedicated cubemap rendering.

---

## How Everything Fits Together

1. Scene and FBOs are initialized.
2. Camera updates each frame via input.
3. Render pipeline executes 5 passes.
4. Lighting, texturing, and fog applied in Pass 1.
5. Bloom and tone mapping applied in later passes.
6. Final composited image is displayed on screen.

---

## Creative Direction

The scene attempts to replicate the mood of a dark, abandoned industrial environment illuminated by a flickering barrel fire.

Focused on:
- Layered textures
- Atmospheric fog
- High contrast lighting
- HDR + bloom integration
- Subtle animated lighting

---

## Models / Textures

Barrel model - https://polyhaven.com/a/barrel_stove
Roof model - https://www.turbosquid.com/3d-models/broken_wall-1396131
Barrier model - https://polyhaven.com/a/concrete_road_barrier_02
Textures - https://cc0-textures.com/

---

## YouTube Video
YouTube Link:
https://youtu.be/WMOf_cz8flA