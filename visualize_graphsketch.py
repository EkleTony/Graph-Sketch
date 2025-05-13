import pandas as pd
import matplotlib.pyplot as plt

# Load anomaly scores from score.txt
file_path = "score.txt"  # Ensure score.txt exists
data = []

try:
    with open(file_path, "r") as file:
        for line in file:
            parts = line.strip().split(",")  # Format: timestamp,score
            if len(parts) == 2:
                timestamp, score = int(parts[0]), float(parts[1])
                data.append((timestamp, score))

    # Convert to DataFrame
    df = pd.DataFrame(data, columns=["Timestamp", "Anomaly Score"])

    # Plot anomaly scores over time
    plt.figure(figsize=(10, 5))
    plt.plot(df["Timestamp"], df["Anomaly Score"], marker="o", linestyle="-", color="b", label="Anomaly Score")
    plt.xlabel("Timestamp")
    plt.ylabel("Anomaly Score")
    plt.title("GraphSketch Anomaly Scores Over Time")
    plt.legend()
    plt.grid(True)

    # Highlight high anomalies
    threshold = df["Anomaly Score"].quantile(0.95)  # Top 5% threshold
    anomalies = df[df["Anomaly Score"] > threshold]
    plt.scatter(anomalies["Timestamp"], anomalies["Anomaly Score"], color="r", label="High Anomalies", zorder=3)

    plt.legend()
    plt.show()

except FileNotFoundError:
    print(f"Error: {file_path} not found! Make sure score.txt exists.")
except Exception as e:
    print(f"Error processing file: {e}")
