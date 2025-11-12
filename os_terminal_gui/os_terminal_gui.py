import sys
import os
import socket
import struct
import json
from datetime import datetime
from PyQt6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout, QPlainTextEdit, QLineEdit,
    QPushButton, QMessageBox, QColorDialog, QComboBox, QLabel
)
from PyQt6.QtGui import QFont, QColor, QPixmap


HOST = "127.0.0.1"
PORT = 31337


class Terminal(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("OS Project Terminal")
        self.resize(900, 650)

        # ===== Default Theme and Font =====
        self.dark_mode = False
        self.font_size = 13
        self.font_family = "Consolas"
        self.text_color = QColor("#000000")

        # ===== Socket =====
        self.sock = None

        # ===== Layout =====
        main_layout = QVBoxLayout()

        # --- Toolbar (Top Buttons) ---
        toolbar = QHBoxLayout()
        self.screenshot_btn = QPushButton("📸 Screenshot")
        self.theme_btn = QPushButton("🌈 Theme")
        self.font_inc_btn = QPushButton("A+")
        self.font_dec_btn = QPushButton("A-")
        self.font_color_btn = QPushButton("🎨 Font Color")

        self.font_selector = QComboBox()
        self.font_selector.addItems(["Consolas", "Courier New", "Monospace"])
        self.font_selector.setCurrentText(self.font_family)

        toolbar.addWidget(self.screenshot_btn)
        toolbar.addWidget(self.theme_btn)
        toolbar.addWidget(self.font_inc_btn)
        toolbar.addWidget(self.font_dec_btn)
        toolbar.addWidget(QLabel("Font:"))
        toolbar.addWidget(self.font_selector)
        toolbar.addWidget(self.font_color_btn)

        main_layout.addLayout(toolbar)

        # --- Output Area ---
        self.output = QPlainTextEdit()
        self.output.setReadOnly(True)
        main_layout.addWidget(self.output)

        # --- Input Area ---
        self.input = QLineEdit()
        self.input.setPlaceholderText("Type your command and press Enter...")
        main_layout.addWidget(self.input)

        self.setLayout(main_layout)

        # ===== Event Bindings =====
        self.screenshot_btn.clicked.connect(self.take_screenshot)
        self.theme_btn.clicked.connect(self.toggle_theme)
        self.font_inc_btn.clicked.connect(self.increase_font)
        self.font_dec_btn.clicked.connect(self.decrease_font)
        self.font_color_btn.clicked.connect(self.choose_font_color)
        self.font_selector.currentTextChanged.connect(self.change_font_family)
        self.input.returnPressed.connect(self.send_command)

        # ===== Initialize =====
        self.connect_backend()
        self.apply_theme()
        self.apply_font()

    # -------------------------------------------------
    # BACKEND CONNECTION
    # -------------------------------------------------
    def connect_backend(self):
        try:
            self.sock = socket.create_connection((HOST, PORT))
            self.output.appendPlainText(f"✅ Connected to {HOST}:{PORT}\n")
        except Exception as e:
            self.output.appendPlainText(f"❌ Connection failed: {e}\n")

    # -------------------------------------------------
    # COMMAND EXECUTION
    # -------------------------------------------------
    def send_command(self):
        cmd = self.input.text().strip()
        if not cmd or not self.sock:
            return

        payload = json.dumps({"command": cmd}).encode()
        header = struct.pack("<I", len(payload))
        try:
            self.sock.sendall(header + payload)
            self.output.appendPlainText(f"project@OS$ {cmd}")
            data = self.sock.recv(8192)
            if data:
                self.output.appendPlainText(data.decode(errors="ignore"))
        except Exception as e:
            self.output.appendPlainText(f"[Error] {e}\n")

        self.input.clear()

    # -------------------------------------------------
    # SCREENSHOT FEATURE
    # -------------------------------------------------
    def take_screenshot(self):
        os.makedirs("screenshots", exist_ok=True)
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = f"screenshots/screenshot_{timestamp}.png"
        pixmap = self.grab()
        pixmap.save(filename)
        QMessageBox.information(self, "Screenshot Saved", f"Saved as:\n{filename}")

    # -------------------------------------------------
    # THEME SWITCHING
    # -------------------------------------------------
    def toggle_theme(self):
        self.dark_mode = not self.dark_mode
        self.apply_theme()

    def apply_theme(self):
        if self.dark_mode:
            self.setStyleSheet("""
                QWidget { background-color: #1e1e1e; color: white; }
                QPlainTextEdit, QLineEdit { background-color: #252526; color: white; border: 1px solid #3e3e3e; }
                QPushButton { background-color: #3a3d41; color: white; border-radius: 5px; padding: 5px; }
                QPushButton:hover { background-color: #444; }
                QComboBox { background-color: #2b2b2b; color: white; }
            """)
        else:
            self.setStyleSheet("""
                QWidget { background-color: #f5f5f5; color: black; }
                QPlainTextEdit, QLineEdit { background-color: white; color: black; border: 1px solid #ccc; }
                QPushButton { background-color: #ddd; color: black; border-radius: 5px; padding: 5px; }
                QPushButton:hover { background-color: #ccc; }
                QComboBox { background-color: white; color: black; }
            """)

    # -------------------------------------------------
    # FONT CUSTOMIZATION
    # -------------------------------------------------
    def apply_font(self):
        font = QFont(self.font_family, self.font_size)
        self.output.setFont(font)
        self.input.setFont(font)
        # Apply font color to output area text
        self.output.setStyleSheet(f"color: {self.text_color.name()};")

    def increase_font(self):
        self.font_size += 1
        self.apply_font()

    def decrease_font(self):
        if self.font_size > 8:
            self.font_size -= 1
            self.apply_font()

    def change_font_family(self, family):
        self.font_family = family
        self.apply_font()

    def choose_font_color(self):
        color = QColorDialog.getColor(initial=self.text_color, parent=self)
        if color.isValid():
            self.text_color = color
            self.apply_font()


# -------------------------------------------------
# MAIN FUNCTION
# -------------------------------------------------
def main():
    app = QApplication(sys.argv)
    win = Terminal()
    win.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
