import numpy as np
import json
import math

color_map = {
    "red": "head",
    "green": "back",
    "blue": "right_Uleg",
    "yellow": "right_Lleg",
    "pink": "left_Uleg",
    "brown": "left_Lleg",
    "cyan": "left_Uarm",
    "purple": "left_Larm",
    "grey": "right_Uarm",
    "black": "right_Larm"
}

limb_values = {
    # populate this
}

joint_rotations = {
    "frames": []
    # populate this
}

joint_map = {
    "neck": ["back", "head"],
    "left_shoulder": ["back", "left_Uarm"],
    "right_shoulder": ["back", "right_Uarm"],
    "left_arm": ["left_Uarm", "left_Larm"],
    "right_arm": ["right_Uarm", "right_Larm"],
    "left_leg": ["back", "left_Uleg"],
    "right_leg": ["back", "right_Uleg"],
    "left_knee": ["left_Uleg", "left_Lleg"],
    "right_knee": ["right_Uleg", "right_Lleg"]
}

numFiles = 2
for frameNum in range(1,numFiles+1):

    f = open("./output/data" + str(frameNum) + ".txt","r")
    lines = f.readlines() 
    for line in lines:
        val = line.split()
        color = val[0]
        x0 = int(val[1])
        y0 = int(val[2])
        x1 = int(val[3])
        y1 = int(val[4])
        limb_values[color_map[color]] = [[x0, y0], [x1, y1]]
    f.close()

    joint_rotations['frames'].append({})
    for joint in joint_map:
        limb_base = joint_map[joint][0]
        limb_end  = joint_map[joint][1]

        base_pts0, base_pts1 = limb_values[limb_base]
        end_pts0,  end_pts1  = limb_values[limb_end]

        base_pts0 = np.asarray(base_pts0)
        base_pts1 = np.asarray(base_pts1)
        end_pts0 = np.asarray(end_pts0)
        end_pts1 = np.asarray(end_pts1)

        dist00 = np.linalg.norm(base_pts0 - end_pts0)
        dist01 = np.linalg.norm(base_pts0 - end_pts1)
        dist10 = np.linalg.norm(base_pts1 - end_pts0)
        dist11 = np.linalg.norm(base_pts1 - end_pts1)

        dist = [dist00, dist01, dist10, dist11]
        min_idx = np.argmin(dist)


        a = 0
        b = 0
        c = 0
        d = 0
        # b and c are closest points
        if(min_idx == 0):
            a = base_pts1
            b = base_pts0
            c = end_pts0
            d = end_pts1
        elif(min_idx == 1):
            a = base_pts1
            b = base_pts0
            c = end_pts1
            d = end_pts0
        elif(min_idx == 2):
            a = base_pts0
            b = base_pts1
            c = end_pts0
            d = end_pts1
        else:
            a = base_pts0
            b = base_pts1
            c = end_pts1
            d = end_pts0

        diff = b - a
        a += diff
        b += diff

        ab = (b - a) / np.sqrt(np.sum((b-a)**2))
        cd = (d - c) / np.sqrt(np.sum((d-c)**2))

        angleX = 0.0 #TODO
        angleY = 0.0 #TODO
        angleZ = math.acos( np.dot(ab,cd) )

        ab3D = np.zeros(3)
        cd3D = np.zeros(3)
        ab3D[0:2] = ab
        cd3D[0:2] = cd

        
        if(False):
            angleZ = 2 * math.pi - angleZ
        




        if((joint == "left_shoulder") or (joint == "right_shoulder")):
            angleZ -= 0.5 * math.pi
            
        joint_rotations['frames'][frameNum-1][joint] = [angleX, angleY, angleZ]

    
with open('../export/rotations.json', 'w') as fp:
    json.dump(joint_rotations, fp, indent=2)