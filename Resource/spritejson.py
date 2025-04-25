from PIL import Image
import cv2
import numpy as np
import json

def extract_sprites_from_png(png_path, json_output_path):
    # 이미지 불러오기 (RGBA)
    image = Image.open(png_path).convert("RGBA")
    image_np = np.array(image)
    
    # 알파 채널 추출 (투명도 기준으로 스프라이트 감지)
    alpha = image_np[:, :, 3]
    
    # 임계값 조정 시도
    _, binary = cv2.threshold(alpha, 1, 255, cv2.THRESH_BINARY)
    
    # OpenCV 버전에 따른 findContours 처리 (버전 차이 대응)
    contours_result = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    # OpenCV 3.x와 4.x 버전 차이 처리
    if len(contours_result) == 3:
        _, contours, _ = contours_result
    else:
        contours, _ = contours_result
    
    # 디버깅용 출력
    print(f"Found {len(contours)} contours")
    
    # 스프라이트 정보 추출
    sprites = []
    for contour in contours:
        x, y, w, h = cv2.boundingRect(contour)
        
        # 너무 작은 윤곽선 필터링 (노이즈 제거)
        if w < 5 or h < 5:
            continue
            
        sprites.append({
            "x": int(x),
            "y": int(y),
            "width": int(w),
            "height": int(h)
        })
    
    # 왼쪽 위에서부터 순서대로 정렬 (먼저 y좌표로, 같은 행에서는 x좌표로)
    sprites.sort(key=lambda s: (s["y"], s["x"]))
    
    # 정렬된 스프라이트에 이름 부여
    sprite_data = []
    for i, sprite in enumerate(sprites):
        sprite_data.append({
            "name": f"sprite_{i}",
            "x": sprite["x"],
            "y": sprite["y"],
            "width": sprite["width"],
            "height": sprite["height"]
        })
        
        # 디버깅: 발견된 스프라이트 정보 출력
        print(f"Sprite {i}: x={sprite['x']}, y={sprite['y']}, w={sprite['width']}, h={sprite['height']}")
    
    # JSON 저장
    with open(json_output_path, 'w') as f:
        json.dump(sprite_data, f, indent=4)
    
    print(f"Saved {len(sprite_data)} sprites to {json_output_path}")

# 예시 사용
extract_sprites_from_png("bricks.png", "bricks.json")
