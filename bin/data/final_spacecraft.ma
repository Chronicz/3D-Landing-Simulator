//Maya ASCII 2026 scene
//Name: final_spacecraft.ma
//Last modified: Wed, Dec 10, 2025 08:49:43 PM
//Codeset: 1252
requires maya "2026";
requires -nodeType "polyDisc" "modelingToolkit" "0.0.0.0";
requires "stereoCamera" "10.0";
requires "mtoa" "5.5.3";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2026";
fileInfo "version" "2026";
fileInfo "cutIdentifier" "202507081222-4d6919b75c";
fileInfo "osv" "Windows 11 Home v2009 (Build: 26100)";
fileInfo "UUID" "20B04C18-4A71-DEEB-F432-8D94ACD0995A";
createNode transform -n "pCube1";
	rename -uid "C58D7FFA-4E4D-29A4-9AC7-6B848AF6B96A";
	setAttr ".s" -type "double3" 4.5094994793110486 1.330271895531614 6.8093864410468408 ;
createNode mesh -n "pCubeShape1" -p "pCube1";
	rename -uid "59BACF59-4B32-E7A0-54E4-EC84C9C609A5";
	addAttr -ci true -h true -sn "_gbp" -ln "gpuBlockPolicy" -at "short";
	setAttr -k off ".v";
	setAttr -s 12 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.375 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr -s 2 ".clst";
	setAttr ".clst[0].clsn" -type "string" "SculptFreezeColorTemp";
	setAttr ".clst[1].clsn" -type "string" "SculptMaskColorTemp";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".lev" 4;
	setAttr -s 16 ".pt[148:163]" -type "float3"  0 0 4.4703484e-08 0 0 4.4703484e-08 
		0 0 7.4505806e-09 0 0 7.4505806e-09 0 0 -7.4505806e-09 0 0 -5.2154064e-08 0 0 1.1175871e-08 
		0 0 -7.4505806e-09 0 0 -5.9604645e-08 0 0 -5.9604645e-08 0 0 1.8626451e-09 0 0 1.8626451e-09 
		0 0 -7.4505806e-09 0 0 -5.2154064e-08 0 0 -7.4505806e-09 0 0 1.1175871e-08;
	setAttr ".dr" 3;
	setAttr ".dsm" 2;
createNode place3dTexture -n "place3dTexture1";
	rename -uid "5A8112C7-4257-3990-E5AB-DABE512D0B48";
createNode place3dTexture -n "place3dTexture2";
	rename -uid "E87392DC-4E1B-C5B3-5DAB-26B4813AE792";
createNode place3dTexture -n "place3dTexture3";
	rename -uid "79153ED0-436B-9A29-D711-E4A8906E110A";
createNode transform -n "pSphere1";
	rename -uid "8F269418-4B38-A832-2859-1DB38962B189";
	setAttr ".t" -type "double3" 0 2.6670190705904364 0 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	rename -uid "24E75F93-4A7A-CDA6-6BAB-A2AD0B59B5AA";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pDisc1";
	rename -uid "C45DA5C7-48DF-EC69-084E-99A3ABEEFE4A";
	setAttr ".t" -type "double3" -0.4542346144459648 2.6774893868058891 0.95313851548509254 ;
	setAttr ".r" -type "double3" 97.547586784660552 3.1805546814635168e-15 -87.50650691582284 ;
	setAttr ".s" -type "double3" 0.16881888164782236 1 0.16881888164782236 ;
createNode mesh -n "pDiscShape1" -p "pDisc1";
	rename -uid "104DF80F-4736-8187-A0E6-C1A1604C6D71";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pDisc2";
	rename -uid "15FBA835-4FC3-71BF-80C5-3A82DE26A171";
	setAttr ".t" -type "double3" 0.23714406714648417 2.7335974547090718 1.0275204709421792 ;
	setAttr ".r" -type "double3" 97.547586784660552 3.1805546814635168e-15 -87.50650691582284 ;
	setAttr ".s" -type "double3" 0.16881888164782236 1 0.16881888164782236 ;
createNode mesh -n "pDiscShape2" -p "pDisc2";
	rename -uid "21B2D511-43A2-66D0-8D97-BE853A29DEF9";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 61 ".uvst[0].uvsp[0:60]" -type "float2" 0.066987306 0.24999997
		 0.93301272 0.25000006 0.49999991 1 0.5 0 0.93301266 0.75000012 0.066987246 0.74999994
		 0.49999997 0.5 0 0.49999994 0.25 0.066987276 0.5 0.25 0.28349364 0.625 0.28349364
		 0.37499997 0.75000006 0.066987306 1 0.50000006 0.7165063 0.62500006 0.71650636 0.37500003
		 0.74999988 0.93301272 0.24999994 0.93301266 0.49999997 0.75 0.17524043 0.6875 0.017037064
		 0.62940949 0.14174682 0.43749997 0.28349364 0.49999997 0.15849361 0.56249994 0.017037094
		 0.37059045 0.14644662 0.14644659 0.26674682 0.22099364 0.15012023 0.29799679 0.37059051
		 0.017037064 0.5 0.125 0.39174682 0.3125 0.38337344 0.17299682 0.5 0.375 0.39174682
		 0.5625 0.39174682 0.4375 0.62940955 0.017037064 0.73325318 0.2209937 0.60825318 0.3125
		 0.61662662 0.17299682 0.85355341 0.14644665 0.98296297 0.37059054 0.85825318 0.43750006
		 0.84987974 0.29799688 0.98296291 0.62940961 0.82475948 0.68750006 0.71650636 0.50000006
		 0.84150636 0.56250006 0.60825312 0.5625 0.60825318 0.43750003 0.85355335 0.85355347
		 0.62499994 0.84150636 0.60825312 0.6875 0.72487968 0.76450324 0.62940943 0.98296297
		 0.37059039 0.98296291 0.37499994 0.84150636 0.49999994 0.90400636 0.14644653 0.85355335
		 0.39174679 0.6875 0.2751202 0.76450312 0.49999997 0.625;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 61 ".vt[0:60]"  -0.86602539 0 0.50000006 0.86602545 0 0.49999991
		 -1.6292068e-07 0 -1 5.9604645e-08 0 1 0.86602533 0 -0.50000018 -0.86602551 0 -0.49999991
		 -3.4438681e-08 0 0 -1 0 1.0323827e-07 -0.49999997 0 0.86602545 1.2582982e-08 0 0.5
		 -0.43301275 0 -0.24999994 -0.43301272 0 0.25000006 0.50000012 0 0.86602539 1 0 -1.5485742e-07
		 0.43301263 0 -0.25000009 0.43301272 0 0.24999993 0.49999982 0 -0.86602551 -0.50000012 0 -0.86602533
		 -8.3115474e-08 0 -0.5 -0.64951915 0 -0.37499994 -0.96592587 0 -0.25881895 -0.71650636 0 0.12500007
		 -0.43301272 0 5.9604645e-08 -0.68301278 0 -0.12499993 -0.96592581 0 0.25881913 -0.70710677 0 0.70710683
		 -0.46650636 0 0.55801272 -0.69975954 0 0.40400642 -0.25881901 0 0.96592587 3.6093812e-08 0 0.75
		 -0.21650636 0 0.37500003 -0.23325315 0 0.65400636 -1.092785e-08 0 0.25 -0.21650639 0 -0.12499997
		 -0.21650638 0 0.12500003 0.25881913 0 0.96592587 0.46650639 0 0.5580126 0.21650636 0 0.37499997
		 0.23325321 0 0.65400636 0.70710683 0 0.70710671 0.96592587 0 0.25881892 0.71650636 0 0.12499989
		 0.69975954 0 0.40400624 0.96592581 0 -0.25881922 0.64951897 0 -0.37500012 0.43301266 0 -8.1956387e-08
		 0.68301266 0 -0.12500013 0.2165063 0 -0.12500004 0.21650633 0 0.12499996 0.70710671 0 -0.70710695
		 0.24999987 0 -0.68301272 0.21650627 0 -0.37500006 0.44975939 0 -0.52900642 0.25881886 0 -0.96592587
		 -0.25881919 0 -0.96592581 -0.25000009 0 -0.68301266 -1.3253758e-07 0 -0.80801272
		 -0.70710695 0 -0.70710671 -0.21650642 0 -0.37499997 -0.44975963 0 -0.5290063 -5.8031773e-08 0 -0.25;
	setAttr -s 108 ".ed[0:107]"  22 10 1 10 19 1 19 23 1 23 22 1 19 5 1 5 20 0
		 20 23 1 20 7 0 7 21 1 21 23 1 21 11 1 11 22 1 7 24 0 24 27 1 27 21 1 24 0 0 0 25 0
		 25 27 1 25 8 0 8 26 1 26 27 1 26 11 1 8 28 0 28 31 1 31 26 1 28 3 0 3 29 1 29 31 1
		 29 9 1 9 30 1 30 31 1 30 11 1 9 32 1 32 34 1 34 30 1 32 6 1 6 33 1 33 34 1 33 10 1
		 22 34 1 37 9 1 29 38 1 38 37 1 3 35 0 35 38 1 35 12 0 12 36 1 36 38 1 36 15 1 15 37 1
		 12 39 0 39 42 1 42 36 1 39 1 0 1 40 0 40 42 1 40 13 0 13 41 1 41 42 1 41 15 1 13 43 0
		 43 46 1 46 41 1 43 4 0 4 44 1 44 46 1 44 14 1 14 45 1 45 46 1 45 15 1 14 47 1 47 48 1
		 48 45 1 47 6 1 32 48 1 37 48 1 51 14 1 44 52 1 52 51 1 4 49 0 49 52 1 49 16 0 16 50 1
		 50 52 1 50 18 1 18 51 1 16 53 0 53 56 1 56 50 1 53 2 0 2 54 0 54 56 1 54 17 0 17 55 1
		 55 56 1 55 18 1 17 57 0 57 59 1 59 55 1 57 5 0 19 59 1 10 58 1 58 59 1 58 18 1 33 60 1
		 60 58 1 47 60 1 51 60 1;
	setAttr -s 48 -ch 192 ".fc[0:47]" -type "polyFaces" 
		f 4 0 1 2 3
		mu 0 4 22 10 19 23
		f 4 4 5 6 -3
		mu 0 4 19 5 20 23
		f 4 7 8 9 -7
		mu 0 4 20 7 21 23
		f 4 10 11 -4 -10
		mu 0 4 21 11 22 23
		f 4 -9 12 13 14
		mu 0 4 21 7 24 27
		f 4 15 16 17 -14
		mu 0 4 24 0 25 27
		f 4 18 19 20 -18
		mu 0 4 25 8 26 27
		f 4 21 -11 -15 -21
		mu 0 4 26 11 21 27
		f 4 -20 22 23 24
		mu 0 4 26 8 28 31
		f 4 25 26 27 -24
		mu 0 4 28 3 29 31
		f 4 28 29 30 -28
		mu 0 4 29 9 30 31
		f 4 31 -22 -25 -31
		mu 0 4 30 11 26 31
		f 4 -30 32 33 34
		mu 0 4 30 9 32 34
		f 4 35 36 37 -34
		mu 0 4 32 6 33 34
		f 4 38 -1 39 -38
		mu 0 4 33 10 22 34
		f 4 -12 -32 -35 -40
		mu 0 4 22 11 30 34
		f 4 40 -29 41 42
		mu 0 4 37 9 29 38
		f 4 -27 43 44 -42
		mu 0 4 29 3 35 38
		f 4 45 46 47 -45
		mu 0 4 35 12 36 38
		f 4 48 49 -43 -48
		mu 0 4 36 15 37 38
		f 4 -47 50 51 52
		mu 0 4 36 12 39 42
		f 4 53 54 55 -52
		mu 0 4 39 1 40 42
		f 4 56 57 58 -56
		mu 0 4 40 13 41 42
		f 4 59 -49 -53 -59
		mu 0 4 41 15 36 42
		f 4 -58 60 61 62
		mu 0 4 41 13 43 46
		f 4 63 64 65 -62
		mu 0 4 43 4 44 46
		f 4 66 67 68 -66
		mu 0 4 44 14 45 46
		f 4 69 -60 -63 -69
		mu 0 4 45 15 41 46
		f 4 -68 70 71 72
		mu 0 4 45 14 47 48
		f 4 73 -36 74 -72
		mu 0 4 47 6 32 48
		f 4 -33 -41 75 -75
		mu 0 4 32 9 37 48
		f 4 -50 -70 -73 -76
		mu 0 4 37 15 45 48
		f 4 76 -67 77 78
		mu 0 4 51 14 44 52
		f 4 -65 79 80 -78
		mu 0 4 44 4 49 52
		f 4 81 82 83 -81
		mu 0 4 49 16 50 52
		f 4 84 85 -79 -84
		mu 0 4 50 18 51 52
		f 4 -83 86 87 88
		mu 0 4 50 16 53 56
		f 4 89 90 91 -88
		mu 0 4 53 2 54 56
		f 4 92 93 94 -92
		mu 0 4 54 17 55 56
		f 4 95 -85 -89 -95
		mu 0 4 55 18 50 56
		f 4 -94 96 97 98
		mu 0 4 55 17 57 59
		f 4 99 -5 100 -98
		mu 0 4 57 5 19 59
		f 4 -2 101 102 -101
		mu 0 4 19 10 58 59
		f 4 103 -96 -99 -103
		mu 0 4 58 18 55 59
		f 4 -102 -39 104 105
		mu 0 4 58 10 33 60
		f 4 -37 -74 106 -105
		mu 0 4 33 6 47 60
		f 4 -71 -77 107 -107
		mu 0 4 47 14 51 60
		f 4 -86 -104 -106 -108
		mu 0 4 51 18 58 60;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode polyDisc -n "polyDisc1";
	rename -uid "4EFC594E-4A48-EE46-FE63-3B8CF83C693B";
createNode materialInfo -n "materialInfo2";
	rename -uid "B9017E8A-43D0-8A71-09D3-20AFEBAB6228";
createNode shadingEngine -n "lambert2SG";
	rename -uid "61CF6D5E-4D93-1DBD-3A62-76AE6B0905AC";
	setAttr ".ihi" 0;
	setAttr -s 4 ".dsm";
	setAttr ".ro" yes;
createNode lambert -n "lambert2";
	rename -uid "65E3B33C-4A57-3875-61C1-509CDC851CE0";
	setAttr ".c" -type "float3" 0 0 0 ;
createNode groupId -n "groupId1";
	rename -uid "D1037F97-4E66-FE82-0F32-49B49C56417E";
	setAttr ".ihi" 0;
createNode shadingEngine -n "lambert3SG";
	rename -uid "545DD15E-47C0-41A0-1E62-998D57924BCB";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "materialInfo3";
	rename -uid "76439416-425B-9169-8B44-E288EB9D6937";
createNode lambert -n "lambert3";
	rename -uid "82930137-4DCA-1C1C-D71D-77AD68D6D409";
createNode marble -n "marble1";
	rename -uid "31FFBF56-4118-44CD-DCF6-BF8750CB9AD0";
	setAttr ".fc" -type "float3" 0 0.15700001 0.13888481 ;
	setAttr ".vc" -type "float3" 0.041999999 0.041999999 0.041999999 ;
	setAttr ".vw" 0.77514791488647461;
	setAttr ".di" 0.36094674468040466;
	setAttr ".c" 0;
createNode groupId -n "groupId3";
	rename -uid "04DDBBD1-44AB-F07E-C392-A3BC56036FF3";
	setAttr ".ihi" 0;
createNode shadingEngine -n "lambert1SG";
	rename -uid "82688612-4339-F7F0-4BDB-2A832BE2109B";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "E1AEC0E7-4C9F-1D56-EF21-1EA7527A47DA";
createNode groupId -n "groupId4";
	rename -uid "D8358751-43C3-A8A4-69EB-A9A0E0F208BF";
	setAttr ".ihi" 0;
createNode groupId -n "groupId5";
	rename -uid "463BA163-4CB7-A7FC-DFEE-91A7803337BC";
	setAttr ".ihi" 0;
createNode groupId -n "groupId6";
	rename -uid "135EFE94-409F-21AD-4021-D3984B84E964";
	setAttr ".ihi" 0;
createNode shadingEngine -n "lambert4SG";
	rename -uid "7F8C70BA-4E05-DD8C-5C41-0792DB36FD6E";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
	rename -uid "A8AAEB5D-485C-ED10-7F97-76B10CB4DE3B";
createNode lambert -n "lambert4";
	rename -uid "C294B337-453B-F46D-592B-D5BA2D994970";
createNode solidFractal -n "solidFractal1";
	rename -uid "FAF9093C-4329-7B46-C3C5-008FB57F78AF";
	setAttr ".cg" -type "float3" 0 0.161 0.14242327 ;
	setAttr ".dc" -type "float3" 0 0.15700001 0.13888481 ;
createNode groupId -n "groupId7";
	rename -uid "763FDE92-49D9-D1FB-C2FA-3F966264EA22";
	setAttr ".ihi" 0;
createNode shadingEngine -n "lambert5SG";
	rename -uid "D10B505E-4BB9-F565-521B-FCB9FE15C312";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
	rename -uid "6691A0BB-44FF-8BBD-BBEC-27B461A75D7F";
createNode lambert -n "lambert5";
	rename -uid "02521BA2-4E8F-7994-0F7C-91A1D5222D53";
	setAttr ".c" -type "float3" 0.84609997 1 0 ;
	setAttr ".it" -type "float3" 0.23870967 0.23870967 0.23870967 ;
	setAttr ".ambc" -type "float3" 0.45806453 0.45806453 0.45806453 ;
	setAttr ".ic" -type "float3" 0.19354838 0.19354838 0.19354838 ;
createNode polyExtrudeFace -n "polyExtrudeFace19";
	rename -uid "663A7556-4414-5FAF-905F-57B84BFA7F50";
	setAttr ".ics" -type "componentList" 1 "f[82:85]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" -0.0065745329 3.099683 -0.031524107 ;
	setAttr ".rs" 59057;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.1403501806710366 1.1859171707439833 -3.3865453048840348 ;
	setAttr ".cbx" -type "double3" 2.1272011150258248 5.0134488220679865 3.323497091494791 ;
createNode polyTweak -n "polyTweak15";
	rename -uid "15D8B414-4DD6-419C-347E-1995E5A4FFC5";
	setAttr ".uopa" yes;
	setAttr -s 48 ".tk[100:147]" -type "float3"  2.8312206e-07 0 -1.4901161e-08
		 1.4901161e-07 0 -1.4901161e-08 -1.7881393e-07 0 3.7252903e-09 -5.9604645e-08 0 3.7252903e-09
		 2.8312206e-07 0 0 1.4901161e-07 0 0 -5.9604645e-08 0 0 -1.7881393e-07 0 0 3.4272671e-07
		 0 0 3.4272671e-07 0 0 1.6391277e-07 0 3.7252903e-09 1.6391277e-07 0 -7.4505806e-09
		 -5.9604645e-08 0 0 -5.9604645e-08 0 0 -4.4703484e-08 0 -7.4505806e-09 -4.4703484e-08
		 0 3.7252903e-09 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 -0.5722599 -0.084499329 0 -0.5722599 -0.084499329 0 -0.0093337158
		 -0.084499329 0 -0.0093337158 -0.084499329;
createNode groupParts -n "groupParts6";
	rename -uid "CA5D44F5-4022-F872-D932-EBB119E6C4E5";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0]";
createNode groupParts -n "groupParts5";
	rename -uid "F6CDCB5A-4C47-3084-FCCC-A4A12E89101B";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 11 "f[1:2]" "f[4:18]" "f[20]" "f[22]" "f[24]" "f[26]" "f[28]" "f[30]" "f[32]" "f[86:89]" "f[94:145]";
	setAttr ".irc" -type "componentList" 1 "f[0]";
createNode groupParts -n "groupParts4";
	rename -uid "D62FE57D-441C-29FD-6770-88AC1C883717";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 8 "f[27]" "f[29]" "f[31]" "f[33:49]" "f[66:75]" "f[77:79]" "f[81]" "f[90:93]";
createNode createColorSet -n "createColorSet2";
	rename -uid "206F4BD7-4FC8-B99D-00AB-0B96F8AB25BA";
	setAttr ".colos" -type "string" "SculptMaskColorTemp";
	setAttr ".clam" no;
createNode createColorSet -n "createColorSet1";
	rename -uid "07109B69-4EB8-D554-9E15-93A037019F6A";
	setAttr ".colos" -type "string" "SculptFreezeColorTemp";
	setAttr ".clam" no;
createNode polyNormalPerVertex -n "polyNormalPerVertex1";
	rename -uid "12512318-4A8F-A5D5-1F52-5CB0DF9B5DDB";
	setAttr ".uopa" yes;
createNode groupParts -n "groupParts3";
	rename -uid "8BA11167-401E-CEB7-8030-DEB335E2F1A6";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 5 "f[3]" "f[19]" "f[21]" "f[23]" "f[25]";
createNode groupParts -n "groupParts2";
	rename -uid "74139467-4E11-0551-61C9-02A632FF3149";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[82:85]";
createNode groupParts -n "groupParts1";
	rename -uid "D405C4FF-42AF-BBAC-F09E-E2B79E672EAB";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 3 "f[50:65]" "f[76]" "f[80]";
	setAttr ".irc" -type "componentList" 4 "f[0:49]" "f[66:75]" "f[77:79]" "f[81:145]";
createNode polyExtrudeFace -n "polyExtrudeFace18";
	rename -uid "AC33FB2D-43EC-09D5-B472-87A34DA85E55";
	setAttr ".ics" -type "componentList" 1 "f[2]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -0.077603363 -3.4046931 ;
	setAttr ".rs" 39195;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.3537010280166903 -1.1144856533452685 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 2.3537010280166903 0.95927892947089799 -3.4046932205234204 ;
createNode polyTweak -n "polyTweak14";
	rename -uid "24DADD3A-4C02-DAC0-255D-5888FAE7F93C";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[136:143]" -type "float3"  0 -0.39493287 -0.085542478
		 0 0.062050894 -0.085542478 0 -0.29498121 -0.085542478 0 -0.12917539 -0.085542478
		 0 -0.39493287 -0.085542478 0 0.062050894 -0.085542478 0 -0.12917539 -0.085542478
		 0 -0.29498121 -0.085542478;
createNode polyExtrudeFace -n "polyExtrudeFace17";
	rename -uid "1D372818-43A4-17F1-745E-4C81A882CDBA";
	setAttr ".ics" -type "componentList" 2 "f[11]" "f[17]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -0.077603363 -3.4046931 ;
	setAttr ".rs" 60595;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -4.8943074767498214 -1.1144856533452685 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 4.8943074767498214 0.95927892947089799 -3.4046932205234204 ;
createNode polyTweak -n "polyTweak13";
	rename -uid "CDD44620-4971-1BAF-1F09-B2A635A8D4D0";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk[132:135]" -type "float3"  0.136843 0.14163961 0.040611476
		 -0.136843 0.14163961 0.040611476 -0.136843 -0.14163961 0.040611476 0.136843 -0.14163961
		 0.040611476;
createNode polyExtrudeFace -n "polyExtrudeFace16";
	rename -uid "B2B178D3-4CFC-3ABD-2060-F5B3C56F2F69";
	setAttr ".ics" -type "componentList" 1 "f[0]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 0 4.3492355 ;
	setAttr ".rs" 38858;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.2547497396555243 -0.66513594776580698 4.3492355395131339 ;
	setAttr ".cbx" -type "double3" 2.2547497396555243 0.66513594776580698 4.3492355395131339 ;
createNode polyTweak -n "polyTweak12";
	rename -uid "1653F554-4184-9314-1940-D2B2867F8815";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[124:131]" -type "float3"  0 0.37609518 0.07366892 0
		 -0.37609518 0.07366892 0 0.061338782 0.07366892 0 -0.2115757 0.07366892 0 0.37609518
		 0.07366892 0 -0.37609518 0.07366892 0 -0.2115757 0.07366892 0 0.061338823 0.07366892;
createNode polyExtrudeFace -n "polyExtrudeFace15";
	rename -uid "AF4CF390-4BF1-E272-5A52-F5B2CB3A466C";
	setAttr ".ics" -type "componentList" 2 "f[13]" "f[15]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -0.077603281 3.4046931 ;
	setAttr ".rs" 40695;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -4.8943074767498214 -1.1144854947645009 3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 4.8943074767498214 0.95927892947089799 3.4046932205234204 ;
createNode polyTweak -n "polyTweak11";
	rename -uid "325D28A8-4BEA-F1BC-C088-C3955175E4A9";
	setAttr ".uopa" yes;
	setAttr -s 24 ".tk[100:123]" -type "float3"  0 0 -0.011924133 0 0 -0.011924133
		 0 0 -0.011924133 0 0 -0.011924133 0 0 0.0026651188 0 0 0.0026651188 0 0 0.0026651188
		 0 0 0.0026651188 -0.028284809 0 0 -0.028284809 0 0 -0.028284809 0 0 -0.028284809
		 0 0 0.025368638 0 0 0.025368638 0 0 0.025368638 0 0 0.025368638 0 0 -0.027267098
		 0.24640721 -0.096049681 -0.027267098 0.24640721 0.096049681 -0.056699455 0.24640721
		 -0.096049681 -0.056699455 0.24640721 0.096049681 0.027267098 0.24640721 -0.096049681
		 0.027267098 0.24640721 0.096049681 0.056699455 0.24640721 0.096049681 0.056699455
		 0.24640721 -0.096049681;
createNode polyExtrudeFace -n "polyExtrudeFace14";
	rename -uid "02892C47-47BE-DEF6-DB15-72B0080F56F3";
	setAttr ".ics" -type "componentList" 2 "f[12]" "f[16]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 0.73249161 0 ;
	setAttr ".rs" 35354;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -4.688547177519883 0.50570427543649288 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 4.688547177519883 0.95927892947089799 3.4046932205234204 ;
createNode polyTweak -n "polyTweak10";
	rename -uid "34228802-42B9-0EE3-F0B7-2EA7979D178F";
	setAttr ".uopa" yes;
	setAttr -s 16 ".tk";
	setAttr ".tk[2]" -type "float3" 0 0 2.2351742e-08 ;
	setAttr ".tk[3]" -type "float3" 0 0 2.2351742e-08 ;
	setAttr ".tk[4]" -type "float3" 0 0 -2.2351742e-08 ;
	setAttr ".tk[5]" -type "float3" 0 0 -2.2351742e-08 ;
	setAttr ".tk[14]" -type "float3" 0 0 -2.2351742e-08 ;
	setAttr ".tk[15]" -type "float3" 0 0 2.2351742e-08 ;
	setAttr ".tk[18]" -type "float3" 0 0 2.2351742e-08 ;
	setAttr ".tk[19]" -type "float3" 0 0 -2.2351742e-08 ;
	setAttr ".tk[108]" -type "float3" 0 0.17036985 -0.066851154 ;
	setAttr ".tk[109]" -type "float3" 0 0.17036985 0.066851154 ;
	setAttr ".tk[110]" -type "float3" 0 -0.17036983 0.02000352 ;
	setAttr ".tk[111]" -type "float3" 0 -0.17036983 -0.023387613 ;
	setAttr ".tk[112]" -type "float3" 0 0.17036985 -0.066851154 ;
	setAttr ".tk[113]" -type "float3" 0 0.17036985 0.066851154 ;
	setAttr ".tk[114]" -type "float3" 0 -0.17036983 -0.023387613 ;
	setAttr ".tk[115]" -type "float3" 0 -0.17036983 0.02000352 ;
createNode polyExtrudeFace -n "polyExtrudeFace13";
	rename -uid "CAD11DCE-4745-EF17-802E-48B183249317";
	setAttr ".ics" -type "componentList" 2 "f[83]" "f[85]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 3.0996833 0 ;
	setAttr ".rs" 54481;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.2547497396555243 0.95927892947089799 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 2.2547497396555243 5.2400876976641424 3.4046932205234204 ;
createNode polyTweak -n "polyTweak9";
	rename -uid "0FB1EA2C-4427-A06B-B031-209F7E3D1C80";
	setAttr ".uopa" yes;
	setAttr -s 16 ".tk";
	setAttr ".tk[4]" -type "float3" 0 2.9802322e-08 1.8626451e-09 ;
	setAttr ".tk[5]" -type "float3" 0 2.9802322e-08 1.8626451e-09 ;
	setAttr ".tk[86]" -type "float3" 0 -4.4703484e-08 1.8626451e-09 ;
	setAttr ".tk[87]" -type "float3" 0 -4.4703484e-08 1.8626451e-09 ;
	setAttr ".tk[96]" -type "float3" 0 1.4901161e-08 3.7252903e-09 ;
	setAttr ".tk[97]" -type "float3" 0 1.4901161e-08 3.7252903e-09 ;
	setAttr ".tk[98]" -type "float3" 0 -1.4901161e-08 3.7252903e-09 ;
	setAttr ".tk[99]" -type "float3" 0 -1.4901161e-08 3.7252903e-09 ;
	setAttr ".tk[100]" -type "float3" 0.071189113 0.22284792 2.7939677e-09 ;
	setAttr ".tk[101]" -type "float3" -0.071189113 0.22284792 2.7939677e-09 ;
	setAttr ".tk[102]" -type "float3" -0.036815375 -0.22284792 2.7939677e-09 ;
	setAttr ".tk[103]" -type "float3" 0.036815375 -0.22284792 2.7939677e-09 ;
	setAttr ".tk[104]" -type "float3" 0.071189113 0.22284792 2.7939677e-09 ;
	setAttr ".tk[105]" -type "float3" -0.071189113 0.22284792 2.7939677e-09 ;
	setAttr ".tk[106]" -type "float3" 0.036815375 -0.22284792 2.7939677e-09 ;
	setAttr ".tk[107]" -type "float3" -0.036815375 -0.22284792 2.7939677e-09 ;
createNode polyExtrudeFace -n "polyExtrudeFace12";
	rename -uid "EC57B42A-446F-EB47-269C-2087DB077A1E";
	setAttr ".ics" -type "componentList" 2 "f[82]" "f[84]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 3.0996833 0 ;
	setAttr ".rs" 35982;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.2547497396555243 0.95927892947089799 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 2.2547497396555243 5.2400876976641424 3.4046932205234204 ;
createNode polyExtrudeFace -n "polyExtrudeFace11";
	rename -uid "FA32DECB-447B-3AD1-FD81-3089A02C7851";
	setAttr ".ics" -type "componentList" 1 "f[82]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 3.0996833 2.2979059 ;
	setAttr ".rs" 56209;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.2547497396555243 0.95927892947089799 1.1911186273512397 ;
	setAttr ".cbx" -type "double3" 2.2547497396555243 5.2400876976641424 3.4046932205234204 ;
createNode polyTweak -n "polyTweak8";
	rename -uid "46B83470-4ECE-3DE9-0261-F4ADD146BB6A";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk[92:95]" -type "float3"  0.11993736 0.58448535 0.11489511
		 -0.11993736 0.58448535 0.11489511 -0.11993736 0.58448535 -0.11489511 0.11993736 0.58448535
		 -0.11489511;
createNode polyExtrudeFace -n "polyExtrudeFace10";
	rename -uid "8C85B5F5-48F3-8670-2C6B-2391AA6FF758";
	setAttr ".ics" -type "componentList" 1 "f[3]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -0.85492051 0 ;
	setAttr ".rs" 33066;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -1.7088568180881263 -0.85492057368698748 -2.47190817439854 ;
	setAttr ".cbx" -type "double3" 1.7088568180881263 -0.85492041510621986 2.47190817439854 ;
createNode polyTweak -n "polyTweak7";
	rename -uid "8F33AC1C-4436-53DA-DEF0-EAA7F2424CC3";
	setAttr ".uopa" yes;
	setAttr -s 4 ".tk[88:91]" -type "float3"  0.14299686 0.19512182 0.13698518
		 -0.14299686 0.19512182 0.13698518 -0.14299686 0.19512182 -0.13698518 0.14299686 0.19512182
		 -0.13698518;
createNode polyExtrudeFace -n "polyExtrudeFace9";
	rename -uid "34344E08-4F7E-4BD1-31A4-57AFE42E3FB7";
	setAttr ".ics" -type "componentList" 1 "f[3]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -1.1144856 0 ;
	setAttr ".rs" 58044;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.3537010280166903 -1.1144856533452685 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 2.3537010280166903 -1.1144854947645009 3.4046932205234204 ;
createNode polyTweak -n "polyTweak6";
	rename -uid "DE487136-48D0-8D0C-294E-2581F153D1E4";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk";
	setAttr ".tk[8]" -type "float3" 0 0 0.13871184 ;
	setAttr ".tk[9]" -type "float3" 0 0 0.13871184 ;
	setAttr ".tk[10]" -type "float3" 0 0 0.13871184 ;
	setAttr ".tk[11]" -type "float3" 0 0 0.13871184 ;
	setAttr ".tk[84]" -type "float3" 0.09612038 0 -0.21353622 ;
	setAttr ".tk[85]" -type "float3" -0.09612038 0 -0.21353622 ;
	setAttr ".tk[86]" -type "float3" -0.09612038 0 0.15324545 ;
	setAttr ".tk[87]" -type "float3" 0.09612038 0 0.15324545 ;
createNode polyExtrudeFace -n "polyExtrudeFace8";
	rename -uid "43B16397-40ED-CAAE-8735-0C975A7693B8";
	setAttr ".ics" -type "componentList" 1 "f[1]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 0.95927894 0 ;
	setAttr ".rs" 46649;
	setAttr ".lt" -type "double3" 0 -0.29144671419769708 4.280808873803859 ;
	setAttr ".ls" -type "double3" 0.70938981564476278 0.69131714442775694 1 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.2547497396555243 0.95927892947089799 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 2.2547497396555243 0.95927892947089799 3.4046932205234204 ;
createNode polyTweak -n "polyTweak5";
	rename -uid "1871E7D1-4E3D-0973-2795-6FA6ED88A199";
	setAttr ".uopa" yes;
	setAttr -s 20 ".tk";
	setAttr ".tk[2]" -type "float3" 0 0.22111493 0 ;
	setAttr ".tk[3]" -type "float3" 0 0.22111493 0 ;
	setAttr ".tk[4]" -type "float3" 0 0.22111493 0 ;
	setAttr ".tk[5]" -type "float3" 0 0.22111493 0 ;
	setAttr ".tk[68]" -type "float3" -0.0025301846 0.38194337 0.0016652048 ;
	setAttr ".tk[69]" -type "float3" -0.0046151634 0.38376787 0.0016652048 ;
	setAttr ".tk[70]" -type "float3" -0.0046151634 0.38376787 0.00075009465 ;
	setAttr ".tk[71]" -type "float3" -0.0025301846 0.38194337 0.00075009465 ;
	setAttr ".tk[72]" -type "float3" -0.0025301846 0.38194337 -0.0016652048 ;
	setAttr ".tk[73]" -type "float3" -0.0046151634 0.38376787 -0.0016652048 ;
	setAttr ".tk[74]" -type "float3" -0.0025301846 0.38194337 -0.00075009465 ;
	setAttr ".tk[75]" -type "float3" -0.0046151634 0.38376787 -0.00075009465 ;
	setAttr ".tk[76]" -type "float3" 0.0025301846 0.38194337 -0.0016652048 ;
	setAttr ".tk[77]" -type "float3" 0.0046151634 0.38376787 -0.0016652048 ;
	setAttr ".tk[78]" -type "float3" 0.0046151634 0.38376787 -0.00075009465 ;
	setAttr ".tk[79]" -type "float3" 0.0025301846 0.38194337 -0.00075009465 ;
	setAttr ".tk[80]" -type "float3" 0.0025301846 0.38194337 0.0016652048 ;
	setAttr ".tk[81]" -type "float3" 0.0046151634 0.38376787 0.0016652048 ;
	setAttr ".tk[82]" -type "float3" 0.0025301846 0.38194337 0.00075009465 ;
	setAttr ".tk[83]" -type "float3" 0.0046151634 0.38376787 0.00075009465 ;
createNode polyExtrudeFace -n "polyExtrudeFace7";
	rename -uid "41F2E224-4AE1-1B3C-0473-688055D88F5A";
	setAttr ".ics" -type "componentList" 4 "f[19]" "f[21]" "f[23]" "f[25]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -2.8163965 0 ;
	setAttr ".rs" 36589;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -5.6367711348868124 -3.2502831472082412 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 5.6367711348868124 -2.3825099987611624 3.4046932205234204 ;
createNode polyTweak -n "polyTweak4";
	rename -uid "0307E897-4C50-66F9-5F08-B0A23C01B0C3";
	setAttr ".uopa" yes;
	setAttr -s 16 ".tk[52:67]" -type "float3"  0.0015752492 0 -1.4901161e-08
		 0.0028733183 0 -1.4901161e-08 0.0028733183 0 3.7252903e-09 0.0015752492 0 3.7252903e-09
		 0.0015752492 0 1.4901161e-08 0.0028733183 0 1.4901161e-08 0.0015752492 0 -3.7252903e-09
		 0.0028733183 0 -9.3132257e-09 -0.0015752586 0 1.4901161e-08 -0.0028733183 0 1.4901161e-08
		 -0.0028733183 0 -9.3132257e-09 -0.0015752586 0 -3.7252903e-09 -0.0015752586 0 -1.4901161e-08
		 -0.0028733183 0 -1.4901161e-08 -0.0015752586 0 3.7252903e-09 -0.0028733183 0 3.7252903e-09;
createNode polyExtrudeFace -n "polyExtrudeFace6";
	rename -uid "5E5EF056-4811-3EC0-B661-85937F97EDF4";
	setAttr ".ics" -type "componentList" 4 "f[19]" "f[21]" "f[23]" "f[25]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -2.8163967 0 ;
	setAttr ".rs" 47876;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -5.6238139832414102 -3.2502834643697764 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 5.6238139832414102 -2.3825101573419301 3.4046932205234204 ;
createNode polyExtrudeFace -n "polyExtrudeFace5";
	rename -uid "28B4B0D1-4CA4-65E6-87BE-819D442CDC4A";
	setAttr ".ics" -type "componentList" 4 "f[19]" "f[21]" "f[23]" "f[25]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -0.68059897 0 ;
	setAttr ".rs" 45032;
	setAttr ".lt" -type "double3" -6.6613381477509392e-16 -4.4408920985006262e-16 2.256947512378372 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -4.8943069391755927 -1.1144856533452685 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 4.8943069391755927 -0.24671230667223043 3.4046932205234204 ;
createNode polyTweak -n "polyTweak3";
	rename -uid "8AF563CF-4546-6A51-98B1-B5ACEE09775C";
	setAttr ".uopa" yes;
	setAttr -s 16 ".tk";
	setAttr ".tk[0]" -type "float3" -0.021942835 -0.33778778 -1.071021e-08 ;
	setAttr ".tk[1]" -type "float3" 0.021942835 -0.33778775 -1.071021e-08 ;
	setAttr ".tk[6]" -type "float3" -0.021942835 -0.3377879 1.8626451e-09 ;
	setAttr ".tk[7]" -type "float3" 0.021942835 -0.3377879 1.8626451e-09 ;
	setAttr ".tk[12]" -type "float3" 0.045628108 0.194691 3.259629e-09 ;
	setAttr ".tk[13]" -type "float3" 0.045628108 0.19469088 9.3132257e-10 ;
	setAttr ".tk[16]" -type "float3" -0.045628108 0.194691 3.259629e-09 ;
	setAttr ".tk[17]" -type "float3" -0.045628108 0.19469082 9.3132257e-10 ;
	setAttr ".tk[20]" -type "float3" 0.021942835 -0.33778772 1.0244548e-08 ;
	setAttr ".tk[21]" -type "float3" 0.021942835 -0.33778775 -1.3969839e-08 ;
	setAttr ".tk[22]" -type "float3" 0.045628108 0.19469079 2.0489097e-08 ;
	setAttr ".tk[23]" -type "float3" 0.045628108 0.19469094 -5.5879354e-09 ;
	setAttr ".tk[24]" -type "float3" -0.021942835 -0.33778772 1.0244548e-08 ;
	setAttr ".tk[25]" -type "float3" -0.021942835 -0.33778775 -1.3969839e-08 ;
	setAttr ".tk[26]" -type "float3" -0.045628108 0.19469091 -5.5879354e-09 ;
	setAttr ".tk[27]" -type "float3" -0.045628108 0.19469079 2.0489097e-08 ;
createNode polyExtrudeFace -n "polyExtrudeFace4";
	rename -uid "DFFFCFCA-462F-BFA9-DC89-C9BCDC0792F4";
	setAttr ".ics" -type "componentList" 2 "f[10]" "f[14]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -0.58542013 0 ;
	setAttr ".rs" 63205;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -4.688547177519883 -0.66513594776580698 -1.2473160416517051 ;
	setAttr ".cbx" -type "double3" 4.688547177519883 -0.50570427543649288 1.2473160416517051 ;
createNode polyTweak -n "polyTweak2";
	rename -uid "FC3B25BB-406E-DC75-F411-E8BAB3C1D80B";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[20:27]" -type "float3"  0 0 0.31682402 0 0 -0.31682402
		 0 0 0.31682402 0 0 -0.31682402 0 0 0.31682402 0 0 -0.31682402 0 0 -0.31682402 0 0
		 0.31682402;
createNode polyExtrudeFace -n "polyExtrudeFace3";
	rename -uid "DE0A4992-47A0-7CBE-E450-18B164FDBF6C";
	setAttr ".ics" -type "componentList" 2 "f[10]" "f[14]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 -0.58542013 0 ;
	setAttr ".rs" 64506;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -4.688547177519883 -0.66513594776580698 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 4.688547177519883 -0.50570431508168479 3.4046932205234204 ;
createNode polyTweak -n "polyTweak1";
	rename -uid "01CFA5AA-4912-A792-9E27-D7B07DCBDA5C";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk[12:19]" -type "float3"  0.53970456 0.11984891 0 0.53970456
		 0.11984891 0 0.53970456 -0.11984891 0 0.53970456 -0.11984891 0 -0.53970462 0.11984891
		 0 -0.53970462 0.11984891 0 -0.53970462 -0.11984891 0 -0.53970462 -0.11984891 0;
createNode polyExtrudeFace -n "polyExtrudeFace2";
	rename -uid "82676FF9-4CB0-1B44-5EB2-48BD716033F5";
	setAttr ".ics" -type "componentList" 1 "f[4:5]";
	setAttr ".ix" -type "matrix" 4.5094994793110486 0 0 0 0 1.330271895531614 0 0 0 0 6.8093864410468408 0
		 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".rs" 41306;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -2.2547497396555243 -0.66513594776580698 -3.4046932205234204 ;
	setAttr ".cbx" -type "double3" 2.2547497396555243 0.66513594776580698 3.4046932205234204 ;
createNode polyExtrudeFace -n "polyExtrudeFace1";
	rename -uid "8BF46D8B-4812-8D04-2248-879B7BF1DB77";
	setAttr ".ics" -type "componentList" 1 "f[0]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0 0 0.5 ;
	setAttr ".rs" 47456;
	setAttr ".ls" -type "double3" 1 1 2.6155468939916431 ;
	setAttr ".c[0]"  0 1 1;
	setAttr ".cbn" -type "double3" -0.5 -0.5 0.5 ;
	setAttr ".cbx" -type "double3" 0.5 0.5 0.5 ;
createNode polyCube -n "polyCube1";
	rename -uid "33620339-4014-110B-7B5D-278A1B40C506";
	setAttr ".cuv" 4;
createNode groupId -n "groupId2";
	rename -uid "CFDB8D5A-47AD-33C7-8458-B08E458BCF69";
	setAttr ".ihi" 0;
createNode polySphere -n "polySphere1";
	rename -uid "387F0832-4A40-4FD8-D19D-55ADF6C49336";
createNode lightLinker -s -n "lightLinker1";
	rename -uid "7A579CA5-4C11-1ABB-458F-BC93D71E0BBB";
	setAttr -s 7 ".lnk";
	setAttr -s 7 ".slnk";
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
	setAttr ".rtfm" 1;
select -ne :renderPartition;
	setAttr -s 7 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 10 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 3 ".u";
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
	setAttr -s 2 ".tx";
select -ne :lambert1;
	setAttr ".c" -type "float3" 0.24349999 0.48026657 0.5 ;
	setAttr ".it" -type "float3" 0.56050956 0.56050956 0.56050956 ;
	setAttr ".ambc" -type "float3" 0.20382166 0.20382166 0.20382166 ;
	setAttr ".ic" -type "float3" 0.095541403 0.095541403 0.095541403 ;
select -ne :standardSurface1;
	setAttr ".bc" -type "float3" 0.40000001 0.40000001 0.40000001 ;
	setAttr ".sr" 0.5;
select -ne :openPBR_shader1;
	setAttr ".bc" -type "float3" 0.40000001 0.40000001 0.40000001 ;
	setAttr ".sr" 0.5;
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultRenderGlobals;
	addAttr -ci true -h true -sn "dss" -ln "defaultSurfaceShader" -dt "string";
	setAttr ".ren" -type "string" "arnold";
	setAttr ".dss" -type "string" "openPBR_shader1";
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :defaultColorMgtGlobals;
	setAttr ".cfe" yes;
	setAttr ".cfp" -type "string" "<MAYA_RESOURCES>/OCIO-configs/Maya2022-default/config.ocio";
	setAttr ".vtn" -type "string" "ACES 1.0 SDR-video (sRGB)";
	setAttr ".vn" -type "string" "ACES 1.0 SDR-video";
	setAttr ".dn" -type "string" "sRGB";
	setAttr ".wsn" -type "string" "ACEScg";
	setAttr ".otn" -type "string" "ACES 1.0 SDR-video (sRGB)";
	setAttr ".potn" -type "string" "ACES 1.0 SDR-video (sRGB)";
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "groupId1.id" "pCubeShape1.iog.og[0].gid";
connectAttr "lambert3SG.mwc" "pCubeShape1.iog.og[0].gco";
connectAttr "groupId3.id" "pCubeShape1.iog.og[1].gid";
connectAttr "lambert1SG.mwc" "pCubeShape1.iog.og[1].gco";
connectAttr "groupId4.id" "pCubeShape1.iog.og[2].gid";
connectAttr "lambert2SG.mwc" "pCubeShape1.iog.og[2].gco";
connectAttr "groupId5.id" "pCubeShape1.iog.og[3].gid";
connectAttr "lambert3SG.mwc" "pCubeShape1.iog.og[3].gco";
connectAttr "groupId6.id" "pCubeShape1.iog.og[4].gid";
connectAttr "lambert4SG.mwc" "pCubeShape1.iog.og[4].gco";
connectAttr "groupId7.id" "pCubeShape1.iog.og[5].gid";
connectAttr "lambert5SG.mwc" "pCubeShape1.iog.og[5].gco";
connectAttr "polyExtrudeFace19.out" "pCubeShape1.i";
connectAttr "groupId2.id" "pCubeShape1.ciog.cog[0].cgid";
connectAttr "polySphere1.out" "pSphereShape1.i";
connectAttr "polyDisc1.output" "pDiscShape1.i";
connectAttr "lambert2SG.msg" "materialInfo2.sg";
connectAttr "lambert2.msg" "materialInfo2.m";
connectAttr "lambert2.oc" "lambert2SG.ss";
connectAttr "pCubeShape1.iog.og[2]" "lambert2SG.dsm" -na;
connectAttr "pDiscShape1.iog" "lambert2SG.dsm" -na;
connectAttr "pDiscShape2.iog" "lambert2SG.dsm" -na;
connectAttr "groupId4.msg" "lambert2SG.gn" -na;
connectAttr "lambert3.oc" "lambert3SG.ss";
connectAttr "pCubeShape1.iog.og[3]" "lambert3SG.dsm" -na;
connectAttr "pCubeShape1.iog.og[0]" "lambert3SG.dsm" -na;
connectAttr "groupId5.msg" "lambert3SG.gn" -na;
connectAttr "groupId1.msg" "lambert3SG.gn" -na;
connectAttr "lambert3SG.msg" "materialInfo3.sg";
connectAttr "lambert3.msg" "materialInfo3.m";
connectAttr "marble1.msg" "materialInfo3.t" -na;
connectAttr "marble1.oc" "lambert3.c";
connectAttr "place3dTexture1.wim" "marble1.pm";
connectAttr ":lambert1.oc" "lambert1SG.ss";
connectAttr "groupId3.msg" "lambert1SG.gn" -na;
connectAttr "pCubeShape1.iog.og[1]" "lambert1SG.dsm" -na;
connectAttr "lambert1SG.msg" "materialInfo1.sg";
connectAttr ":lambert1.msg" "materialInfo1.m";
connectAttr "lambert4.oc" "lambert4SG.ss";
connectAttr "pCubeShape1.iog.og[4]" "lambert4SG.dsm" -na;
connectAttr "groupId6.msg" "lambert4SG.gn" -na;
connectAttr "lambert4SG.msg" "materialInfo4.sg";
connectAttr "lambert4.msg" "materialInfo4.m";
connectAttr "solidFractal1.msg" "materialInfo4.t" -na;
connectAttr "solidFractal1.oc" "lambert4.c";
connectAttr "place3dTexture3.wim" "solidFractal1.pm";
connectAttr "lambert5.oc" "lambert5SG.ss";
connectAttr "pCubeShape1.iog.og[5]" "lambert5SG.dsm" -na;
connectAttr "pSphereShape1.iog" "lambert5SG.dsm" -na;
connectAttr "groupId7.msg" "lambert5SG.gn" -na;
connectAttr "lambert5SG.msg" "materialInfo5.sg";
connectAttr "lambert5.msg" "materialInfo5.m";
connectAttr "polyTweak15.out" "polyExtrudeFace19.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace19.mp";
connectAttr "groupParts6.og" "polyTweak15.ip";
connectAttr "groupParts5.og" "groupParts6.ig";
connectAttr "groupId7.id" "groupParts6.gi";
connectAttr "groupParts4.og" "groupParts5.ig";
connectAttr "groupId6.id" "groupParts5.gi";
connectAttr "createColorSet2.og" "groupParts4.ig";
connectAttr "groupId5.id" "groupParts4.gi";
connectAttr "createColorSet1.og" "createColorSet2.ig";
connectAttr "polyNormalPerVertex1.out" "createColorSet1.ig";
connectAttr "groupParts3.og" "polyNormalPerVertex1.ip";
connectAttr "groupParts2.og" "groupParts3.ig";
connectAttr "groupId4.id" "groupParts3.gi";
connectAttr "groupParts1.og" "groupParts2.ig";
connectAttr "groupId3.id" "groupParts2.gi";
connectAttr "polyExtrudeFace18.out" "groupParts1.ig";
connectAttr "groupId1.id" "groupParts1.gi";
connectAttr "polyTweak14.out" "polyExtrudeFace18.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace18.mp";
connectAttr "polyExtrudeFace17.out" "polyTweak14.ip";
connectAttr "polyTweak13.out" "polyExtrudeFace17.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace17.mp";
connectAttr "polyExtrudeFace16.out" "polyTweak13.ip";
connectAttr "polyTweak12.out" "polyExtrudeFace16.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace16.mp";
connectAttr "polyExtrudeFace15.out" "polyTweak12.ip";
connectAttr "polyTweak11.out" "polyExtrudeFace15.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace15.mp";
connectAttr "polyExtrudeFace14.out" "polyTweak11.ip";
connectAttr "polyTweak10.out" "polyExtrudeFace14.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace14.mp";
connectAttr "polyExtrudeFace13.out" "polyTweak10.ip";
connectAttr "polyTweak9.out" "polyExtrudeFace13.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace13.mp";
connectAttr "polyExtrudeFace12.out" "polyTweak9.ip";
connectAttr "polyExtrudeFace11.out" "polyExtrudeFace12.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace12.mp";
connectAttr "polyTweak8.out" "polyExtrudeFace11.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace11.mp";
connectAttr "polyExtrudeFace10.out" "polyTweak8.ip";
connectAttr "polyTweak7.out" "polyExtrudeFace10.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace10.mp";
connectAttr "polyExtrudeFace9.out" "polyTweak7.ip";
connectAttr "polyTweak6.out" "polyExtrudeFace9.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace9.mp";
connectAttr "polyExtrudeFace8.out" "polyTweak6.ip";
connectAttr "polyTweak5.out" "polyExtrudeFace8.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace8.mp";
connectAttr "polyExtrudeFace7.out" "polyTweak5.ip";
connectAttr "polyTweak4.out" "polyExtrudeFace7.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace7.mp";
connectAttr "polyExtrudeFace6.out" "polyTweak4.ip";
connectAttr "polyExtrudeFace5.out" "polyExtrudeFace6.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace6.mp";
connectAttr "polyTweak3.out" "polyExtrudeFace5.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace5.mp";
connectAttr "polyExtrudeFace4.out" "polyTweak3.ip";
connectAttr "polyTweak2.out" "polyExtrudeFace4.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace4.mp";
connectAttr "polyExtrudeFace3.out" "polyTweak2.ip";
connectAttr "polyTweak1.out" "polyExtrudeFace3.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace3.mp";
connectAttr "polyExtrudeFace2.out" "polyTweak1.ip";
connectAttr "polyExtrudeFace1.out" "polyExtrudeFace2.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace2.mp";
connectAttr "polyCube1.out" "polyExtrudeFace1.ip";
connectAttr "pCubeShape1.wm" "polyExtrudeFace1.mp";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert5SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert5SG.message" ":defaultLightSet.message";
connectAttr "lambert1SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "lambert5SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert3.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert4.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert5.msg" ":defaultShaderList1.s" -na;
connectAttr "place3dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place3dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place3dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "marble1.msg" ":defaultTextureList1.tx" -na;
connectAttr "solidFractal1.msg" ":defaultTextureList1.tx" -na;
connectAttr "pCubeShape1.ciog.cog[0]" ":initialShadingGroup.dsm" -na;
connectAttr "groupId2.msg" ":initialShadingGroup.gn" -na;
// End of final_spacecraft.ma
