# Save this as fix_tester.py
import sys

# Read the broken tester
with open('kmeans_tester_improved.py', 'r', encoding='utf-8') as f:
    content = f.read()

# Replace all gibberish with proper emoji
replacements = {
    'ðŸ"¨': '🔨',
    'âŒ': '❌',
    'âœ…': '✅',
    'ðŸ§ª': '🧪',
    'âš ï¸': '⚠️',
    'ðŸš€': '🚀',
    'ðŸ†': '🏆',
}

for broken, fixed in replacements.items():
    content = content.replace(broken, fixed)

# Write back
with open('kmeans_tester_improved.py', 'w', encoding='utf-8') as f:
    f.write(content)

print("✅ Fixed! Run: python3 kmeans_tester_improved.py")
