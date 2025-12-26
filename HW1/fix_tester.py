# Save this as fix_tester.py
# -*- coding: utf-8 -*-
import sys

# Read the broken tester
with open('kmeans_tester_improved.py', 'r', encoding='utf-8') as f:
    content = f.read()

# All possible broken emoji patterns
replacements = {
    'ðŸ"¨': '🔨',
    'âŒ': '❌',
    'âœ…': '✅',
    'ðŸ§ª': '🧪',
    'âš ï¸': '⚠️',
    'ðŸš€': '🚀',
    'ðŸ†': '🏆',
    'Ã°ÂŸÂ"Â¨': '🔨',
    'Ã¢ÂœÂ…': '✅',
    'Ã¢âŒâœ': '❌',
    'Ã¢â‚¬â„¢': '🏆',
    'Ã¢â‚¬â„¢': '🏆',
}

for broken, fixed in replacements.items():
    content = content.replace(broken, fixed)

# Write back with UTF-8 encoding
with open('kmeans_tester_improved.py', 'w', encoding='utf-8') as f:
    f.write(content)

print("✅ Fixed! Run: python3 kmeans_tester_improved.py")
