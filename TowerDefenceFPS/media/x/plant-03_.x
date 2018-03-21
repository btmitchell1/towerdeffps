xof 0303txt 0032
template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}

template FVFData {
 <b6e70a0e-8ef9-4e83-94ad-ecc8b0c04897>
 DWORD dwFVF;
 DWORD nDWords;
 array DWORD data[nDWords];
}

template EffectInstance {
 <e331f7e4-0559-4cc2-8e99-1cec1657928f>
 STRING EffectFilename;
 [...]
}

template EffectParamFloats {
 <3014b9a0-62f5-478c-9b86-e4ac9f4e418b>
 STRING ParamName;
 DWORD nFloats;
 array FLOAT Floats[nFloats];
}

template EffectParamString {
 <1dbc4c88-94c1-46ee-9076-2c28818c9481>
 STRING ParamName;
 STRING Value;
}

template EffectParamDWord {
 <e13963bc-ae51-4c5d-b00f-cfa3a9d97ce5>
 STRING ParamName;
 DWORD Value;
}


Material bridge {
 0.588000;0.588000;0.588000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "bridge.tga";
 }
}

Material tree04 {
 0.588000;0.588000;0.588000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "tree04.tga";
 }
}

Material plant02 {
 0.588000;0.588000;0.588000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "plant02.tga";
 }
}

Material plant01 {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "plant01.tga";
 }
}

Material hedge {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "hedge.tga";
 }
}

Material lighter {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "lighter.tga";
 }
}

Material bench {
 0.588000;0.588000;0.588000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "bench.tga";
 }
}

Material tree-01_barkSub0 {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "bark.tga";
 }
}

Material tree-01_tree01Sub1 {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "tree01.tga";
 }
}

Material tree-02_barkSub0 {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "bark.tga";
 }
}

Material tree-02_tree02Sub1 {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "tree02.tga";
 }
}

Material tree-03_barkSub0 {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "bark.tga";
 }
}

Material tree-03_tree03Sub1 {
 0.588235;0.588235;0.588235;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "tree03.tga";
 }
}

Material fence {
 0.588000;0.588000;0.588000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "fence.tga";
 }
}

Frame plant-03_ {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,-0.581692,4.019352,0.263854,1.000000;;
  }

  Mesh  {
   48;
   5.335327;-1.161150;1.572876;,
   8.265137;3.856062;-4.003967;,
   0.861206;-4.155711;-0.288544;,
   5.335327;-1.161150;1.572876;,
   12.170898;-2.742549;3.259277;,
   8.265137;3.856062;-4.003967;,
   12.170898;-2.742549;3.259277;,
   5.335327;-1.161150;1.572876;,
   3.099854;0.399754;7.456238;,
   5.335327;-1.161150;1.572876;,
   0.861206;-4.155711;-0.288544;,
   3.099854;0.399754;7.456238;,
   0.181152;0.189683;6.285980;,
   5.498047;3.911588;6.266846;,
   0.861206;-4.155711;-0.288544;,
   0.181152;0.189683;6.285980;,
   0.214111;-3.828513;12.088898;,
   5.498047;3.911588;6.266846;,
   0.214111;-3.828513;12.088898;,
   0.181152;0.189683;6.285980;,
   -5.724121;2.840452;5.814087;,
   0.181152;0.189683;6.285980;,
   0.861206;-4.155711;-0.288544;,
   -5.724121;2.840452;5.814087;,
   -6.264038;-0.938762;0.532013;,
   -5.472778;-0.333017;4.963776;,
   0.861206;-4.155711;-0.288544;,
   -6.264038;-0.938762;0.532013;,
   -12.170776;-2.493880;1.355103;,
   -5.472778;-0.333017;4.963776;,
   -12.170776;-2.493880;1.355103;,
   -6.264038;-0.938762;0.532013;,
   -5.474487;3.415832;-3.862762;,
   -6.264038;-0.938762;0.532013;,
   0.861206;-4.155711;-0.288544;,
   -5.474487;3.415832;-3.862762;,
   0.654663;0.038817;-5.094604;,
   -5.349731;3.430073;-4.861938;,
   0.861206;-4.155711;-0.288544;,
   0.654663;0.038817;-5.094604;,
   1.172729;-1.212254;-12.088898;,
   -5.349731;3.430073;-4.861938;,
   1.172729;-1.212254;-12.088898;,
   0.654663;0.038817;-5.094604;,
   5.850952;4.155711;-3.057678;,
   0.654663;0.038817;-5.094604;,
   0.861206;-4.155711;-0.288544;,
   5.850952;4.155711;-3.057678;;
   16;
   3;0,1,2;,
   3;3,4,5;,
   3;6,7,8;,
   3;9,10,11;,
   3;12,13,14;,
   3;15,16,17;,
   3;18,19,20;,
   3;21,22,23;,
   3;24,25,26;,
   3;27,28,29;,
   3;30,31,32;,
   3;33,34,35;,
   3;36,37,38;,
   3;39,40,41;,
   3;42,43,44;,
   3;45,46,47;;

   MeshNormals  {
    48;
    -0.246363;0.960865;0.126663;,
    -0.249649;0.781437;0.571867;,
    -0.542937;0.838614;-0.044114;,
    -0.246363;0.960865;0.126663;,
    0.128141;0.927730;0.350566;,
    -0.249649;0.781437;0.571867;,
    0.128141;0.927730;0.350566;,
    -0.246363;0.960865;0.126663;,
    -0.053627;0.959932;-0.275054;,
    -0.246363;0.960865;0.126663;,
    -0.542937;0.838614;-0.044114;,
    -0.053627;0.959932;-0.275054;,
    -0.099589;0.994657;0.027198;,
    -0.572905;0.818168;-0.048791;,
    -0.056865;0.830181;-0.554586;,
    -0.099589;0.994657;0.027198;,
    -0.097892;0.817931;0.566927;,
    -0.572905;0.818168;-0.048791;,
    -0.097892;0.817931;0.566927;,
    -0.099589;0.994657;0.027198;,
    0.409219;0.912426;0.004403;,
    -0.099589;0.994657;0.027198;,
    -0.056865;0.830181;-0.554586;,
    0.409219;0.912426;0.004403;,
    0.101398;0.929833;0.353735;,
    0.091906;0.984261;-0.150941;,
    0.419974;0.847218;0.325336;,
    0.101398;0.929833;0.353735;,
    -0.197783;0.924179;0.326763;,
    0.091906;0.984261;-0.150941;,
    -0.197783;0.924179;0.326763;,
    0.101398;0.929833;0.353735;,
    0.181999;0.681940;0.708403;,
    0.101398;0.929833;0.353735;,
    0.419974;0.847218;0.325336;,
    0.181999;0.681940;0.708403;,
    -0.099115;0.960735;0.259163;,
    0.486331;0.846108;0.218134;,
    -0.182881;0.736800;0.650907;,
    -0.099115;0.960735;0.259163;,
    -0.009968;0.984200;-0.176782;,
    0.486331;0.846108;0.218134;,
    -0.009968;0.984200;-0.176782;,
    -0.099115;0.960735;0.259163;,
    -0.654851;0.730290;0.194541;,
    -0.099115;0.960735;0.259163;,
    -0.182881;0.736800;0.650907;,
    -0.654851;0.730290;0.194541;;
    16;
    3;0,1,2;,
    3;3,4,5;,
    3;6,7,8;,
    3;9,10,11;,
    3;12,13,14;,
    3;15,16,17;,
    3;18,19,20;,
    3;21,22,23;,
    3;24,25,26;,
    3;27,28,29;,
    3;30,31,32;,
    3;33,34,35;,
    3;36,37,38;,
    3;39,40,41;,
    3;42,43,44;,
    3;45,46,47;;
   }

   MeshMaterialList  {
    1;
    16;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;
    { plant02 }
   }

   MeshTextureCoords  {
    48;
    0.500000;0.356341;,
    1.000000;0.356341;,
    0.503925;1.012605;,
    0.500000;0.356341;,
    0.500241;-0.003684;,
    1.000000;0.356341;,
    0.500241;-0.003684;,
    0.500000;0.356341;,
    0.000000;0.356341;,
    0.500000;0.356341;,
    0.503925;1.012605;,
    0.000000;0.356341;,
    0.500000;0.356341;,
    1.000000;0.356341;,
    0.503925;1.012605;,
    0.500000;0.356341;,
    0.500241;-0.003684;,
    1.000000;0.356341;,
    0.500241;-0.003684;,
    0.500000;0.356341;,
    0.000000;0.356341;,
    0.500000;0.356341;,
    0.503925;1.012605;,
    0.000000;0.356341;,
    0.500000;0.356341;,
    1.000000;0.356341;,
    0.503925;1.012605;,
    0.500000;0.356341;,
    0.500241;-0.003684;,
    1.000000;0.356341;,
    0.500241;-0.003684;,
    0.500000;0.356341;,
    0.000000;0.356341;,
    0.500000;0.356341;,
    0.503925;1.012605;,
    0.000000;0.356341;,
    0.500000;0.356341;,
    1.000000;0.356341;,
    0.503925;1.012605;,
    0.500000;0.356341;,
    0.500241;-0.003684;,
    1.000000;0.356341;,
    0.500241;-0.003684;,
    0.500000;0.356341;,
    0.000000;0.356341;,
    0.500000;0.356341;,
    0.503925;1.012605;,
    0.000000;0.356341;;
   }
  }
 }
}

AnimationSet Anim-1 {
 
}