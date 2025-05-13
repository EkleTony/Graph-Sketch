import numpy as np


def process_score_and_groundtruth(score_path, groundtruth_path, output_path):
    # Load scores only (you assume each row is an anomaly)
    scores = []
    with open(score_path, 'r') as f:
        for line in f:
            parts = line.strip().split(',')
            scores.append(float(parts[-1]))

    # Load full anomaly ground truth (same number of total graph updates, labeled 0/1)
    with open(groundtruth_path, 'r') as f:
        labels = [int(line.strip()) for line in f.readlines()]

    if len(scores) != len(labels):
        print(
            f"[⚠️ Mismatch] Scores: {len(scores)} vs Ground Truth: {len(labels)}")
        return

    # Save aligned version
    with open(output_path, 'w') as out:
        for s, l in zip(scores, labels):
            out.write(f"{s} {l}\n")
    print(f"✅ Saved processed file to: {output_path}")
