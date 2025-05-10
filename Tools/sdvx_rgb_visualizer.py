import pygame
import mmap

# Init pygame
pygame.init()

# LED count configuration
LED_COUNTS = [74, 12, 12, 56, 56, 94, 12, 12, 14, 86]
LED_OFFSETS = [0, 74, 86, 98, 154, 210, 304, 316, 328, 342]
LED_NAMES = ['TITLE', 'UPPER L SP', 'UPPER R SP', 'LEFT WING', 'RIGHT WING', 
            'CTRL PANEL', 'LOWER L SP', 'LOWER R SP', 'WOOFER', 'V UNIT']

# Window settings
LED_SIZE = 10  # Each LED is displayed as 10x10 pixels
STRIP_GAP = 2  # Gap between strips
TEXT_WIDTH = 20  # Text width
WINDOW_WIDTH = (LED_SIZE + STRIP_GAP + TEXT_WIDTH) * len(LED_COUNTS)
WINDOW_HEIGHT = LED_SIZE * max(LED_COUNTS)  # Use the longest strip's height
screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("SDVX LED Visualizer")

font = pygame.font.SysFont('Arial', 12)

# Open shared memory
try:
    shm = mmap.mmap(-1, 1284, "sdvxrgb")
except Exception as e:
    print(f"Failed to open shared memory: {e}")
    exit(1)

clock = pygame.time.Clock()
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
    # Clear screen
    screen.fill((0, 0, 0))
    
    # Read shared memory data and update display
    shm.seek(0)
    data = shm.read()
    
    # Draw each strip
    for strip_idx in range(len(LED_COUNTS)):
        offset = LED_OFFSETS[strip_idx] * 3
        count = LED_COUNTS[strip_idx]
        x = strip_idx * (LED_SIZE + STRIP_GAP + TEXT_WIDTH)
        
        # Draw all LEDs of this strip
        for led_idx in range(count):
            r = data[offset + led_idx * 3]
            g = data[offset + led_idx * 3 + 1]
            b = data[offset + led_idx * 3 + 2]
            y = led_idx * LED_SIZE
            
            # Draw LED
            pygame.draw.rect(screen, (r, g, b), 
                           (x, y, LED_SIZE, LED_SIZE))
        
        # Draw text
        text = LED_NAMES[strip_idx]
        y_text = 5
        for char in text:
            if char != ' ':
                text_surface = font.render(char, True, (255, 255, 255))
                text_rect = text_surface.get_rect()
                text_rect.centerx = x + LED_SIZE + TEXT_WIDTH//2
                text_rect.top = y_text
                screen.blit(text_surface, text_rect)
                y_text += text_rect.height + 1
    
    pygame.display.flip()
    clock.tick(120)  # Limit to 120 frames per second

pygame.quit() 